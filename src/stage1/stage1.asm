;
; Read FAT partition to load stage2
;
; We cannot assume that stage1 is loaded from our MBR
;

; Size of headers before our code
%define FAT_HEADERS_SIZE        62
; End of Line \r\n
%define EOL                     0x0D, 0x0A
; Magic breakpoint from Bochs
%define BREAKPOINT              xchg bx, bx

bits 16 ; Real mode
org FAT_HEADERS_SIZE  ; Because of FAT headers, tell NASM to offset address

begin:
    ; Disable interrupts
    cli 
    ; Reset direction flag
    cld

    ; Our are loaded to 0:0x7C00
    ; we want plenty of continue space to load stage2
    ; so move our self to lowest available memory
    ; Source = DS:SI
    mov ax, 0x07C0  ; 0x07C0:0 = 0x07C0 * 0x10 + 0 = 0x7C00
    mov ds, ax
    xor si, si
    ; Destination = ES:DI
    mov ax, 0x0050  ; 0x0050 is the lowest available memory
    mov es, ax
    xor di, di
    ; our size = 512 bytes
    mov cx, 512
    ; perform copy
    rep movsb

    ; Continue from our new location
    jmp 0x0050:start

start:
    ; because we move to 0x0500, DS should be 0x50
    mov ax, 0x0050
    mov ds, ax
    mov es, ax

    ; Setup the stack
    ; because the stack goes downward, we want the stack point at the max available address (0x7FFFF)
    ; the highest stack pointer is 0xFFFF
    ; so to get address 0x7FFFF, we set the stack segment to 0x7000
    ; so highest memory address will be 0x7000 * 0x10 + 0xFFFF = 0x7FFFF
    mov sp, 0xFFFF
    mov ax, 0x7000
    mov ss, ax

    ; Ready for interrupts
    sti
    
    
    mov si, msg_hello
    call print_string
    
halt:
    cli
    mov si, msg_halt
    call print_string
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

msg_halt:                   db 'Halt!', EOL, 0
msg_hello:                  db 'Hello Stage1!', EOL, 0

end:
; Fill the rest of space with zeros
times 448 - ( end - begin ) nop