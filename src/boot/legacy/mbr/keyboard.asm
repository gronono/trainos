; Wait for a key pressed
; Return the pressed key into AH
wait_key:
    mov ah, 0
    int 16h
    ret

