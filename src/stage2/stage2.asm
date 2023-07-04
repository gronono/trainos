;
; Read FAT partition to load stage2
;
; We cannot assume that stage1 is loaded from our MBR
;

; End of Line \r\n
%define EOL                     0x0D, 0x0A
; Magic breakpoint from Bochs
%define BREAKPOINT              xchg bx, bx
; VGA address
%define SCREEN_BUFFER           0xB8000
[map all /trainos/build/stage2.map]
bits 32 ; Protected mode
org 0x0800

begin:
    ; Reset segment registers to use the 32-bit data segment of GDT
    mov eax, 0x10   ; offset 16
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax
    ; clear direction flag
    cld
    ; enable interrupt
    sti


    ; print hello 
    BREAKPOINT
    mov esi, msg_hello
    call print_string
    
halt:
    cli
    hlt
    jmp halt

;
; Print a zero-terminated string
; Input:
; - esi : points to the string
; Ouput:
; - esi : points to next address after the end of the string
print_string:
    push eax
    push edi

    mov edi, SCREEN_BUFFER
.loop:
    lodsb           ; al = next character, si++
    or al, al       ; test if al = 0 (ie end of string)
    jz .done        ; if true, break the loop
    mov [edi], al   ; copy current character into video buffer
    inc edi
    inc edi
    jmp .loop
.done:
    pop eax
    pop edi
    ret

msg_halt:                   db 'Halt!', EOL, 0
msg_hello:                  db 'Hello Stage2!', EOL, 0

end: