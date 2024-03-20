;
; Create the booloader inside the Volume Boot Record.
;
; The main goal of this file is to switch to protected mode
;
%include "../lib/symbols.asm"

; NASM directives
[map all /trainos/build/boot/legacy/boot.map]
bits 16 ; CPU starts in real mode
org FAT_HEADERS_SIZE   ; Set origin after FAT16 headers

begin:
    %include "../lib/init.asm"

    mov si, msg_welcome
    call print_string

    ; Switch to protected mode
    %include "./switch_protected_mode.asm"

    ; Jump to start.c
    ; we need to offset because we moved to 0x500
    jmp GDT_CODE:(end + 0x500)

%include "../lib/print_string.asm"
msg_welcome     db 'TrainOS VBR', EOL, EOL, 0

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
    db 10011010b ; access (present, ring 0, code segment, executable, direction 0, readable)
    db 11001111b ; granularity (4k pages, 32-bit mode) + limit (bits 16-19)
    db 0         ; base high
gdt_data:
    ; start at 0x16
    dw 0xFFFF    ; limit (bits 0-15) = 0xFFFF for full 32-bit range
    dw 0         ; base (bits 0-15) = 0x0
    db 0         ; base (bits 16-23)
    db 10010010b ; access (present, ring 0, data segment, executable, direction 0, writable)
    db 11001111b ; granularity (4k pages, 32-bit mode) + limit (bits 16-19)
    db 0         ; base high
gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; size (16 bit)
    dd gdt_start + 0x500        ; address (32 bit) - because we tell NASM do not translate address and we move to 0x500, we need to add 0x500.

; don't write anything after end:
end:
