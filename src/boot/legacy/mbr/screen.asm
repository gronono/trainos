;
; Clear the screen by settings Video Mode
;
clear_screen:
    push ax ; al / ah

    mov ah, 0x00    ; 0x00 = Set video mode
    mov al, 0x03    ; text: 80x25, resolution: 640x400, colors 16
    int 10h         ; call BIOS interruption

    pop ax
    ret
