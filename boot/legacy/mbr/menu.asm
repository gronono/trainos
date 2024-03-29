menu_loop:
    call print_menu
    call wait_key
    call handle_key

; Print the menu
print_menu:
    call clear_screen
    ; Display welcome message
    mov si, msg_welcome
    call print_string

    mov cx, 0
    .loop:
        ; print partition line; format '- partition #<num>'
        cmp cx, [selected_menu]
        je .selected

        mov si, '-'
        call print_char
        jmp .continue

        .selected:
            mov si, '>'
            call print_char

        .continue:
            mov si, ' '
            call print_char

            call is_bootable
            jne .end

            mov si, '*'
            call print_char

        .end:
            mov si, msg_partition
            call print_string
            mov si, '0'
            add si, cx
            call print_char

            mov si, msg_crlf
            call print_string

        inc cx
        cmp cx, 3
        jle .loop
    ret

handle_key:
    cmp ax, DOWN_ARROW
    je move_down

    cmp ax, ENTER_KEY
    je select

    jmp menu_loop

move_down:
    inc byte [selected_menu]
    cmp byte [selected_menu], 3
    jle menu_loop
    mov byte [selected_menu], 0
    jmp menu_loop
