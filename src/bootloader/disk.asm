;
; Read disk sectors
; Inputs:
; - dl: drive index
; - cx: number of sectors to read
; - es:bx: destination
; - ax: LBA Address
disk_read:
    push si
    push eax

    ; Source = DS:SI = Address of DAP
    mov si, disk_dap
    
    ; Fill DAP
    mov word  [disk_dap.number_of_sectors], cx
    mov word  [disk_dap.buffer_segment], es
    mov word  [disk_dap.buffer_offset], bx
    mov dword [disk_dap.lba], eax
BREAKPOINT
    ; Call read from LBA
    mov ax, 0x42
    int 0x13
    jc .error
    jmp .success

.error:
    mov si, msg_disk_error
    call print_string
    jmp halt

.success:
    pop eax
    pop si
    ret

msg_disk_error           db 'Disk error!', EOL, 0
disk_dap:
    .packet_size         db  0x10
    .reserved            db  0x00
    .number_of_sectors   dw  0x0001
    .buffer_offset       dw  0x0000
    .buffer_segment      dw  0x0000
    .lba                 dq  0x00000000
