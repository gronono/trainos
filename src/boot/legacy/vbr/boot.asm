;
; Create the booloader inside the Volume Boot Record.
;
; The main goal of this file is to switch to protected mode
;
%include "../lib/symbols.asm"

bits 16 ; CPU starts in real mode
extern start

section .boot

begin:
    %include "../lib/init.asm"
    mov si, msg_welcome
    call print_string

    ; Only the first sector of our VBR is loaded into RAM
    ; We need to load the next sectors
    ; 0x500 (our location) is the start of the partition
    mov eax, [ORIGIN_ADDR + FAT_HIDDEN_SECTORS_OFFSET]
    add eax, 1  ;   next sector
    mov [disk_dap.lba], eax
    mov si, disk_dap
    mov dl, 0x80
    mov ah, 0x42
    int 0x13
    jnc after_read
    mov si, msg_disk_error
    call print_string
after_read:
    ; Switch to protected mode
    %include "./switch_protected_mode.asm"

    ; Jump to start.c
    jmp GDT_CODE:start

%include "../lib/print_string.asm"
msg_welcome     db 'TrainOS VBR', EOL, EOL, 0
msg_disk_error  db 'Disk Error!', EOL, 0

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

; Disk Address Packet
disk_dap:
    .size       db  0x10
    .reserved   db  0x00
    ; number of sectors (1 sector = 512 bytes)
    .sectors    dw  0x0001
    ; destination = 0x0000:0x7C00
    .offset     dw  0x0700
    .segment    dw  0x0000
    ; LBA Address = index (zero based) of the first sector to read
    .lba        dq  0x00000000
