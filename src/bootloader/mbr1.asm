;
; Create the bootloader inside the Master Boot Record.
; This is the first part of the bootloader located on disk at 0x0000-0x00DA.
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
    BREAKPOINT
    ; Because we are not ready
    ; Disable interrupts
    cli

    ; BIOS load us at 0:0x7C00
    ; To simplify address offset, we want to use offset from 0x7C00
    ; so we use segment at 0x07C0 (addr = segment * 0x10 + offset)
    ; to set Code Segment, we need to jmp
    jmp 0x07C0:setup

setup:
    ; now code segment = 0x07C0
    ; we align other segments
    mov ax, 0x07C0
    mov ds, ax  ; data segment
    mov es, ax  ; extra segment
    
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
 
 copy:
    ; Because we will the bootable partition at standard address 0x7C00
    ; we need to copy our self to lowest available memory
    ; movsb copy from ds:si to es:di
    ; so set ds to your origin
    mov ax, 0x07C0
    mov ds, ax
    ; and set es to your destination
    mov ax, 0x0050
    mov es, ax
    ; and set si and di offsets to 0
    mov si, 0
    mov di, 0
    ; movsb use cx as counter, 512 bytes is our size
    mov cx, 512                 ; counter = 512 bytes = our size
    rep movsb                   ; repeat cx time: copy 1 byte from ds:si to es:di and increment si, di by one

    ; Continue from our new location
    jmp 0x0050:continue

continue:
    ; now code segment = 0x0050
    ; we align other segments
    mov ax, 0x0050
    mov ds, ax  ; data segment
    mov es, ax  ; extra segment

    ;
    ; Lookup for a bootable partition
    ;
    mov cx, 4       ; counter for looping over partitions
    mov si, 0x01BE  ; First partition
check_partition:
    ; check if the first byte of the partition entry contains the boot flag
    mov ax, [si]                    ; ax = content of the 1st byte
    and ax, 0b10000000              ; and with boot flag mask (first bit)
    cmp ax, 0b10000000              ; compare if ax is the flag
    je load_vbr                     ; if it is the case, jump to load_vbr
    add si, 16                      ; next partition (each partition entry is 16 bytes long)
    loop check_partition            ; loop
    
   mov si, msg_no_bootable_partition
   call print_string
   jmp halt

load_vbr:
    mov si, msg_found
    call print_string

halt:
    cli
    mov si, msg_halt
    call print_string
    hlt

%include "print_string.asm"

msg_halt                    db 'Halt!', EOL, 0
msg_no_bootable_partition:  db 'No bootable partition!', EOL, 0
msg_found:                  db 'trouve!', EOL, 0

end:
; Fill the rest of space with zeros
; the first section of the MBR is 218 bytes
times 218 - ( end - begin ) db 0