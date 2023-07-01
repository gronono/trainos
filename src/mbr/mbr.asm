;
; Create the bootloader inside the Master Boot Record.
;
; Search for the first active partition and jump to its bootloader.
;

bits 16 ; This bootloader is executed on real mode so in 16 bits
org 0   ; Tell nasm do not translate addresses

; End of Line \r\n
%define EOL                     0x0D, 0x0A
; Magic breakpoint from Bochs
%define BREAKPOINT              xchg bx, bx

begin:
    ; Because we are not ready
    ; Disable interrupts
    cli
    
    ; Because we will copy the bootable partition at standard address 0x7C00
    ; we need to copy our self to lowest available memory
    ; movsb copy from DS:SI to ES:DI
    ; set DS from our source
    mov ax, 0x07C0  ; 0x07C0:0 = 0x07C0 * 0x10 + 0 = 0x7C00
    mov ds, ax
    ; set es to our destination
    mov ax, 0x0050  ; 0x0050 is the lowest available memory
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
    ; because we move to 0x0500, DS should be 0x50
    mov ax, 0x0050
    mov ds, ax

    ; Setup the stack
    ; because the stack goes downward, we want the stack point at the max available address (0x7FFFF)
    ; the highest stack pointer is 0xFFFF
    ; so to get address 0x7FFFF, we set the stack segment to 0x7000
    ; so highest memory address will be 0x7000 * 0x10 + 0xFFFF = 0x7FFFF
    mov sp, 0xFFFF
    mov ax, 0x7000
    mov ss, ax

    ; Reset direction flag, so we know this value
    cld
    
    ; Setup finished we enable interruption
    sti
    
    ;
    ; Lookup for a bootable partition
    ;
    mov cx, 4       ; counter for looping over partitions
    mov si, 0x01BE  ; First partition
loop_partition:
    ; check if the first byte of the partition entry contains the boot flag
    mov ax, [si]                    ; ax = content of the 1st byte
    and ax, 0b10000000              ; and with boot flag mask (first bit)
    cmp ax, 0b10000000              ; compare if ax is the flag
    je load_vbr                     ; if it is the case, jump to load_VBR
    add si, 16                      ; next partition (each partition entry is 16 bytes long)
    loop loop_partition             ; loop; decrement CX until 0 then continue
    
    ; No partition boot flag found on all 4 partitions
    ; Display a message and halt
    mov si, msg_no_bootable_partition
    call print_string
    jmp halt

load_vbr:
    ; Fill DAP
    ; Read first sector of the partition (ie VBR)
    mov word [disk_dap.number_of_sectors], 1
    ; destination = 0:07C00
    mov word [disk_dap.buffer_segment], 0
    mov word [disk_dap.buffer_offset], 0x7C00
    ; LBA of the partition = offset of 0x08 from partition entry
    add si, 0x08
    mov eax, [si]
    mov dword [disk_dap.lba], eax
    ; Source = DS:SI = Addres of DAP
    ; DS is already set at 0x50
    mov si, disk_dap

    ; Call read from LBA
    mov ah, 0x42
    int 0x13
    jc disk_error

    ; Jump to VBR
    jmp 0:0x7C00

halt:
    cli
    mov si, msg_halt
    call print_string
    hlt

disk_error:
    mov si, msg_disk_error
    call print_string
    jmp halt

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

msg_halt:                   db 'Halt!', EOL, 0
msg_no_bootable_partition:  db 'No bootable partition!', EOL, 0
msg_yep:                    db 'YEP :)', 0
msg_disk_error              db 'Disk error!', EOL, 0
disk_dap:
    .packet_size            db  0x10
    .reserved               db  0x00
    .number_of_sectors      dw  0x0001
    .buffer_offset          dw  0x0000
    .buffer_segment         dw  0x0000
    .lba                    dq  0x00000000

end:
; Fill the rest of space with zeros
times 440 - ( end - begin ) db 0
