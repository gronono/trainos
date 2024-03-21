;
; Switch to protected mode
;

; Enable A20
call a20_wait_input
mov al, KdbDisable
out KdbCommandPort, al

call a20_wait_input
mov al, KdbReadPort
out KdbCommandPort, al

call a20_wait_output
in al, KdbDataPort
push eax

call a20_wait_input
mov al, KdbWritePort
out KdbCommandPort, al

call a20_wait_input
pop eax
or al, 2            ; bit 2 = A20 bit
out KdbDataPort, al

call a20_wait_input
mov al, KdbEnable
out KdbCommandPort, al

call a20_wait_input

; load Global Descriptor Table (aligned with physical memory)
lgdt [gdt_descriptor]
; set protection enable flag in CR0
mov eax, cr0
or eax, 1
mov cr0, eax

; Far jump to protected mode
; In protected mode, address are formed by segment:offset
; where segment is the entry offset in GDT, and offset is the offset in this entry.
; Because we aligned GDT segments with physical memory
; we jump to the code segment with an offset.
; The next instruction is located at end_protected_mode
jmp GDT_CODE:end_protected_mode

a20_wait_input:
    ; wait until status bit 2 (input buffer) is 0
    ; by reading from command port, we read status byte
    in al, KdbCommandPort
    test al, 2
    jnz a20_wait_input
    ret

a20_wait_output:
    ; wait until status bit 1 (output buffer) is 1 so it can be read
    in al, KdbCommandPort
    test al, 1
    jz a20_wait_output
    ret

end_protected_mode:
 ; From here we are now in 32 bits
 bits 32
    ; Reset stack
    mov ebp, 0x7FFFF
    mov esp, ebp
    mov ax, GDT_DATA
    mov ss, ax

