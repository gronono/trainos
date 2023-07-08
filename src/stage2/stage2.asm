;
; Read FAT partition to load stage2
;

; End of Line \r\n
%define EOL                     0x0D, 0x0A
; Magic breakpoint from Bochs
%define BREAKPOINT              xchg bx, bx
; VGA address
%define SCREEN_BUFFER           0xB8000

bits 32 ; Protected mode
section .entry

extern start

global entry
entry:
    ; Reset segment registers to use the 32-bit data segment of GDT
    mov eax, 0x10   ; offset 16 (3rd segement)
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax
    ; clear direction flag
    cld

    ; Call start in main.c
    BREAKPOINT
    call start
    
halt:
    cli
    hlt
    jmp halt
