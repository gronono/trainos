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
.check_partition:
    ; check if the first byte of the partition entry contains the boot flag
    mov ax, [si]                    ; ax = content of the 1st byte
    and ax, 0b10000000              ; and with boot flag mask (first bit)
    cmp ax, 0b10000000              ; compare if ax is the flag
    je .load_vbr                    ; if it is the case, jump to load_vbr
    add si, 16                      ; next partition (each partition entry is 16 bytes long)
    loop .check_partition           ; loop; decrement CX until 0 then continue
    
    ; No partition boot flag found on all 4 partitions
    ; Display a message and halt
    mov si, msg_no_bootable_partition
    call print_string
    jmp halt

.load_vbr:
    ; Read first sector of the partition (ie VBR)
    mov cx, 1   ; 1 sector
    ; destination = ES:BX = 0:07C00
    mov ax, 0
    mov es, ax
    mov bx, 0x7C00
    ; LBA of the partition = offset of 0x08 from partition entry
    add si, 0x08
    mov eax, [si]
    call disk_read
    
    BREAKPOINT
    mov si, msg_yep
    call print_string
    jmp halt
    
halt:
    cli
    mov si, msg_halt
    call print_string
    hlt

%include "print.asm"
%include "disk.asm"

msg_halt:                   db 'Halt!', EOL, 0
msg_no_bootable_partition:  db 'No bootable partition!', EOL, 0
msg_yep:                    db 'YEP :)', 0

end:
; Fill the rest of space with zeros
times 440 - ( end - begin ) db 0
