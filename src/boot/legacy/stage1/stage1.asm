;
; Create the bootloader inside the Master Boot Record.
; Load and execute stage2
;
[map all /trainos/build/boot/legacy/stage1.map]
bits 16 ; This bootloader is executed on real mode so in 16 bits
org 0   ; Tell nasm do not translate addresses

; End of Line \r\n
%define EOL                         0x0D, 0x0A
; Magic breakpoint from Bochs
%define BREAKPOINT                  xchg bx, bx

; Some Keyboard controller constantes
%define KdbDataPort                 0x60
%define KdbCommandPort              0x64
%define KdbDisable                  0xAD
%define KdbEnable                   0xAE
%define KdbReadPort                 0xD0
%define KdbWritePort                0xD1

begin:
    ; Because we are not ready
    ; Disable interrupts
    cli

    ; Copy our self 0x5600
    ; movsb copy from DS:SI to ES:DI
    ; set DS from our source
    mov ax, 0x07C0  ; 0x07C0:0 = 0x07C0 * 0x10 + 0 = 0x7C00
    mov ds, ax
    ; set es to our destination
    mov ax, 0x0050  ; 0x0500:0 = 0x0050 x 0x10 + 0 = 0x500
    mov es, ax
    ; and set si and di offsets to 0
    mov si, 0
    mov di, 0
    ; counter = 512 bytes = our size
    mov cx, 512
    ; repeat cx time: copy 1 byte from ds:si to es:di and increment si, di by one
    rep movsb

    ; Continue from our new location
    jmp 0x0050:start

start:
    ; Setup Data-Segment
    ; Because we move to 0x0500, data will be at 0x0050:offset
    mov ax, 0x0050
    mov ds, ax

    ; Setup Stack (goes downward)
    ; To avoid conflict between our code and the stack
    ; setup at the max available
    ; 0x7FFFF = 0x7000 x 0x10 + 0xFFFF
    mov ax, 0x7000
    mov ss, ax
    mov sp, 0xFFFF
    mov bp, sp

    ; Reset direction flag, so we know this value
    cld

    ; Setup finished we enable interruption
    sti

    ; Load stage 2
    ; Fill DAP
    mov ax, [stage2_size]
    mov word [disk_dap.number_of_sectors], ax
    ; destination = 0:0x700 (0x500 + 512)
    mov word [disk_dap.buffer_segment], 0
    mov word [disk_dap.buffer_offset], 0x700
    ; LBA of the partition = 2nd sector = 1
    mov eax, 1
    mov dword [disk_dap.lba], eax
    ; Source = DS:SI = Address of DAP
    ; DS is already set
    mov si, disk_dap

    ; Call read from LBA
    mov ah, 0x42
    int 0x13
    jc disk_error

    ;
    ; Switch to protected mode
    ;
    call enable_a20
    ; load Global Descriptor Table
    lgdt [gdt_descriptor]
    ; set protection enable flag in CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump to stage 2
    ; in protected mode, address are formed by segment:offset
    ; where segment is the entry offset in GDT, and offset is the offset in this entry
    ; because we aligned GDT segments with actual memory
    ; we jump to the second segment with the offset of stage 2.
    jmp 0x08:0x0700   ; 0x08 = 2nd segment, 0x0700 = stage2

disk_error:
    mov si, msg_disk_error
    call print_string
    cli
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

enable_a20:
    cli

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
    ret

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

gdt_start:
    ; NULL descriptor
    dq 0
gdt_code:
    dw 0xFFFF    ; limit (bits 0-15) = 0xFFFF for full 32-bit range
    dw 0x0       ; base (bits 0-15) = 0x0
    db 0x0       ; base (bits 16-23)
    db 10011010b ; access (present, ring 0, code segment, executable, direction 0, readable)
    db 11001111b ; granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
    db 0x0       ; base high
gdt_data:
    dw 0xFFFF    ; limit (bits 0-15) = 0xFFFF for full 32-bit range
    dw 0x0       ; base (bits 0-15) = 0x0
    db 0x0       ; base (bits 16-23)
    db 10010010b ; access (present, ring 0, data segment, executable, direction 0, writable)
    db 11001111b ; granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
    db 0x0       ; base high
gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; size (16 bit)
    dd gdt_start + 0x500        ; address (32 bit) - beacuse we tell NASM do not translate address and we move to 0x500, we need to add 0x500.

msg_halt:                   db 'Halt!', EOL, 0
msg_disk_error              db 'Disk error!', EOL, 0
disk_dap:
    .packet_size            db  0x10
    .reserved               db  0x00
    .number_of_sectors      dw  0x0001
    .buffer_offset          dw  0x0000
    .buffer_segment         dw  0x0000
    .lba                    dq  0x00000000

end:

; = MBR size - signature length - 4 partition entries of 16 bytes - 2 bytes reserved for stage2 length
; Fill the rest of space with zeros
times 440 - ( end - begin ) - 2 db 0
stage2_size: ; 2 bytes

