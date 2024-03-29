;
; Create the booloader inside the Volume Boot Record.
;
; The main goal of this file is to switch to protected mode
;
[map all /trainos/build/boot/legacy/vbr.map]
bits 16      ; CPU starts in real mode
org 0x053E   ; Will moved to 0x500 so set origin to that + offset by FAT16 headers (0x3E)

%include "../lib/symbols.asm"

%define MEMORY_MAP_ADDR   0x8000

begin:
    %include "../lib/init.asm"

    mov si, msg_welcome
    call print_string

    ; Load kernel into RAM
    mov si, kernel_dap
    mov dl, 0x80
    mov ah, 0x42
    int 0x13
    jnc after_read
    mov si, msg_disk_error
    call print_string
after_read:
    ; Detect memory map before switching to protected mode
    ; because our address is high, we need to set DS
    ; MEMORY_MAP_ADDR = 0x7B9FF = 0x7000*10 + 0xB9FF = DS * 10 + MEMORY_MAP_ADDR
    ;mov ax, 0x7000
    ;mov ds, ax
    %include "./memory_map.asm"
    ;mov ds, bx      ; memory map completed so ebx=0

    ; Switch to protected mode
    %include "./switch_protected_mode.asm"

    ; Jump to kmain.c
    jmp GDT_CODE:0x700

%include "../lib/print_string.asm"
msg_welcome     db 'TrainOS VBR', EOL, EOL, 0
msg_disk_error  db 'Disk Error!', EOL, 0
msg_smap_failed db 'Memory Map Failed!', EOL, 0
;
; Global Descriptor Table (GDT)
; map 1 to 1 with physical memory
; 3 tables: 0 = null, 0x08 = code and 0x16 = data
gdt_start:
    ; NULL descriptor (8 bytes)
    dq 0
gdt_code:
    ; start at 0x08
    dw 0xFFFF    ; limit (bits 0-15) = 0xFFFF for full 32-bit range
    dw 0         ; base (bits 0-15) = 0x00
    db 0         ; base (bits 16-23)
    db 10011010b ; (=0x9A) access (present, ring 0, code segment, executable, direction 0, readable)
    db 11001111b ; (=0xCF) granularity (4k pages, 32-bit mode) + limit (bits 16-19)
    db 0         ; base high
gdt_data:
    ; start at 0x10
    dw 0xFFFF    ; limit (bits 0-15) = 0xFFFF for full 32-bit range
    dw 0         ; base (bits 0-15) = 0x0
    db 0         ; base (bits 16-23)
    db 10010010b ; access (present, ring 0, data segment, executable, direction 0, writable)
    db 11001111b ; granularity (4k pages, 32-bit mode) + limit (bits 16-19)
    db 0         ; base high
gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; size (16 bit)
    dd gdt_start                ; address (32 bit)

; Kernel Disk Address Packet
kernel_dap:
    .size       db  0x10
    .reserved   db  0x00
    ; number of sectors to read (1 sector = 512 bytes)
    .sectors    dw  0x001A  ;   max around 3FD  FIXME Dynamic size
    ; destination = 0x0000:0x7C00
    .offset     dw  0x0700  ; TODO ? Move just behind VBR
    .segment    dw  0x0000
    ; LBA Address = index (zero based) of the first sector to read
    .lba        dq  0x0000086C  ; value is set on compile FIXME dynamic address

end:
; Fill the rest of space with zeros
times 450 - ( end - begin ) db 0
