;
; Create the bootloader inside the Master Boot Record.
; Search for the first active partition and jump to its bootloader.
;

; This bootloader is executed on real mode so in 16 bits
bits 16

; Available memory
%define MIN_AVAILABLE_MEM   0x00500
%define MAX_AVAILABLE_MEN   0x7FFFF
; Stack segment
; stack pointer is form by ss:sp
; We'll set the stack segment at 0x7000 and the highest pointer to 0xFFFF
; so highest stack pointer is 0x7000 * 0x10 + 0xFFFF = 0x7FFFF
%define STACK_SEGMENT       (MAX_AVAILABLE_MEN - MAX_STACK_POINTER) / 0x10
%define MAX_STACK_POINTER   0xFFFF
; BIOS load bootloader at this address
%define BOOTLOADER_ADDRESS  0x7C00
; Location after copy our self
%define NEXT_LOCATION       MIN_AVAILABLE_MEM
; MBR = 512 bytes
%define MBR_SIZE            512
; End of Line \r\n
%define EOL                 0x0D, 0x0A

begin:
    ; Because our are not ready
    ; Disable interrupts
    cli

    ; Setup the stack
    mov ax, STACK_SEGMENT
    mov ss, ax
    mov sp, MAX_STACK_POINTER

    ; Reset some registers
    mov ax, 0  
    mov ds, ax ; data segment
    mov es, ax ; extra segmant
    cld        ; direction flag
    
    ; Setup finished we enable interruption
    sti
 
    ; We will load the bootable partition at standard address 0x7C00
    ; so copy our self to lowest available memory
    mov si, BOOTLOADER_ADDRESS  ; source = our current address
    mov di, NEXT_LOCATION       ; destination = lowest available memory
    mov cx, MBR_SIZE            ; counter = 512 bytes = our size
    rep movsb                   ; repeat cx time: copy 1 byte from ds:si to es:di and increment si, di by one

    ; Continue from our new location
    jmp 0:(NEXT_LOCATION + resume)

resume:
    ; Display Hello world
    mov si, NEXT_LOCATION + msg_hello
    call print_string

    mov si, NEXT_LOCATION + no_bootable_partition
    call print_string

halt:
    cli
    hlt

%include "print_string.asm"

msg_hello:              db 'Hello from MBR!', EOL, 0
no_bootable_partition:  db 'No bootable partition!', 0

end:
; Fill the rest of space with zeros
times MBR_SIZE  -2 - ( end - begin ) db 0
; Boot signature
dw 0xAA55