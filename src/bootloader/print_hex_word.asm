; https://stackoverflow.com/a/38323758
; 
; Print 16 bit value passed on stack as first parameter
; in hexadecimal. Use page number and foreground color
; passed in second parameter. This routine will work on 8086+
; processors. This code takes advantage of packed BCD to
; determine the ASCII values to print. This code could have
; used compare and branch to do the same or a translation table.
print_hex_word:
    push bp
    mov bp, sp      ; BP=SP, on 8086 can't use sp in memory operand
    push dx         ; Save all registers we clobber
    push cx
    push bx
    push ax

    mov cx, 0x0404  ; CH = number of nibbles to process = 4 (4*4=16 bits)
                    ; CL = Number of bits to rotate each iteration = 4 (a nibble)
    mov dx, [bp+4]  ; DX = word parameter on stack at [bp+4] to print
    mov bx, [bp+6]  ; BX = page / foreground attr is at [bp+6]

.loop:
    rol dx, cl      ; Roll 4 bits left. Lower nibble is value to print
    mov ax, 0x0e0f  ; AH=0E (BIOS tty print),AL=mask to get lower nibble
    and al, dl      ; AL=copy of lower nibble
    add al, 0x90    ; Work as if we are packed BCD
    daa             ; Decimal adjust after add.
                    ;    If nibble in AL was between 0 and 9, then CF=0 and
                    ;    AL=0x90 to 0x99
                    ;    If nibble in AL was between A and F, then CF=1 and
                    ;    AL=0x00 to 0x05
    adc al, 0x40    ; AL=0xD0 to 0xD9
                    ; or AL=0x41 to 0x46
    daa             ; AL=0x30 to 0x39 (ASCII '0' to '9')
                    ; or AL=0x41 to 0x46 (ASCII 'A' to 'F')
    int 0x10        ; Print ASCII character in AL
    dec ch
    jnz .loop       ; Go back if more nibbles to process

    pop ax          ; Restore registers
    pop bx
    pop cx
    pop dx
    pop bp
    ret