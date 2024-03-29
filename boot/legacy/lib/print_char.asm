; Print an ASCII char.
; Input:
; - SI: ASCII value of the char
print_char:
    push ax
    push bx

    mov ax, si
    mov ah, 0x0E
    mov bh, 0
    int 0x10

    pop bx
    pop ax
    ret
