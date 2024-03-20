; Load Volume Boot Record
load_vbr:
    ; Compute the LBA address
    ; = FIRST_BOOT_FLAG_ADDR + (selected * PARTITION_ENTRY_SIZE) + LBA_ADDR_OFFSET
    ; = 0x1BE + (selected * 10) + 0x08
    ; for the 1st partition : 0x1BE + 0x08 =  0x01C6
    ; should be 0x800 = 2048th sector (because parted alignment)
    ; => located at 0x800 * (size of a sector = 512 = 0x200) = 0x100000
    mov bx, [selected_menu]
    mov eax, PARTITION_ENTRY_SIZE
    mul bx
    add eax, (FIRST_BOOT_FLAG_ADDR + LBA_ADDR_OFFSET)
    mov ebx, eax
    mov eax, dword [ds:ebx]
    mov dword [disk_dap.lba], eax
    ; Source = DS:SI = Address of DAP
    mov si, disk_dap

    ; Call read from LBA
    mov dl, 0x80
    mov ah, 0x42
    int 0x13
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
    ; LBA Address = index (zero based) of the first sector to read
    .lba        dq  0x00000000
