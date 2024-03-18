;
; Read FAT partition to load stage2
;

; End of Line \r\n
%define EOL                     0x0D, 0x0A
; Magic breakpoint from Bochs
%define BREAKPOINT              xchg bx, bx

bits 32 ; Protected mode
section .entry

extern start
extern __bss_start
extern __end
extern _init

global entry
entry:
    ; In protected move, segments use GDT
    ; so we need to reset segments
    mov eax, 0x10 ; offset 16 (3rd segement)
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Reset stack
    mov ebp, 0x7FFFF
    mov esp, ebp

    ; clear direction flag
    cld

    ; clear bss (uninitialized data)
;    mov edi, __bss_start
;    mov ecx, __end
;    sub ecx, edi
;    mov al, 0
;    rep stosb

    ; Call start in main.c
    jmp start

; Never call
halt:
    cli
    hlt
    jmp halt
