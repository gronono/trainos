; Load Volume Boot Record
load_vbr:
    ; Fill DAP
    mov bx, [selected_menu]
    mov eax, PARTITION_ENTRY_SIZE
    mul bx
    add eax, FIRST_BOOT_FLAG_ADDR
    add eax, LBA_ADDR_OFFSET
    mov ebx, eax
    mov eax, dword [ds:ebx]
    mov dword [disk_dap.lba], eax
    ; Source = DS:SI = Address of DAP
    mov si, disk_dap

    ; Call read from LBA
    mov dl, 0x80
    mov ah, 0x42
    int 0x13
    BREAKPOINT
    ret

is_bootable:
    mov ax, PARTITION_ENTRY_SIZE
    mul cx
    add ax, FIRST_BOOT_FLAG_ADDR
    mov bx, ax
    mov bl, [bx]
    cmp bl, BOOT_FLAG_ENABLED
    ret

; Disk Address Packet
disk_dap:
    .size       db  0x10
    .reserved   db  0x00
    ; 1st sector = 512 bytes
    .sectors    dw  0x0001
    ; destination = 0x0000:0x7C00
    .offset     dw  0x7C00
    .segment    dw  0x0000
    .lba        dq  0x00000000
