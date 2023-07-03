;
; Read FAT partition to load stage2
;
; We cannot assume that stage1 is loaded from our MBR
;

; End of Line \r\n
%define EOL                     0x0D, 0x0A
; Magic breakpoint from Bochs
%define BREAKPOINT              xchg bx, bx

bits 16 ; Real mode
org 0   ; Tell nasm do not translate addresses

begin:
    BREAKPOINT

    mov si, msg_hello
    call print_string
    
halt:
    cli
    mov si, msg_halt
    call print_string
    hlt

;
; Print a zero-terminated string by calling 0x10 BIOS interruption
; Input:
; - si : points to the string
; Ouput:
; - si : points to next address after the end of the string
print_string:
    push ax ; al / ah
    push bx ; bh

    .loop:
        lodsb           ; al = next character, si++
        cmp al, 0       ; test if al = 0 (ie end of string)
        je .done        ; if true then jump to done
        mov ah, 0x0E    ; 0x0E = BIOS teletype    
        mov bh, 0       ; set page number to 0
        int 0x10        ; call BIOS interruption
        jmp .loop       ; continue looping
    
    .done:
        pop bx
        pop ax
        ret

msg_halt:                   db 'Halt!', EOL, 0
msg_hello:                  db 'Hello Stage2!', EOL, 0

end: