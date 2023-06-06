; Memory address where BIOS puts us
%define ORG 0x7C00
; End of Line
%define EOL 0x0D, 0x0A

; In bootloader, CPU is in 'Real Mode' = 16 bits
bits 16
; BIOS puts the bootloader at 7C00 memory address
org ORG

; Memory will be address without offset
; So reset segment registers
xor ax, ax  ; ax = 0
mov ds, ax
mov es, ax
mov ss, ax 

; Setup stack
; stack grows downwards from where we are loaded in memory
mov sp, ORG

; Print Hello World
mov si, msg_hello
call print

halt:
    cli     ; Disable interrupts
    hlt     ; Halt

;
; Print a zero-terminated string by calling 0x10 BIOS interruption
; Params: si points to the string
;
print:
    push si
    push ax
    push bx

    .loop:
        lodsb           ; al = next character, si++
        test al, al     ; test if al = 0 (ie end of string)
        jz .done        ; jump to done
        mov ah, 0x0E    ; 0x0E = BIOS teletype    
        mov bh, 0       ; set page number to 0
        int 0x10        ; call BIOS interruption
        jmp .loop       ; continue looping
    
    .done:
        pop bx
        pop ax
        pop si
        ret

; Variables
msg_hello: db 'Hello world!', EOL, 0

; Before writing boot sector magic number, add a bunch a nop operation
; so the bootloader take the required 512 bytes 
; $ is the current address
; $$ is address of start of current section
times 512 - 2 - ( $ - $$ ) nop
; boot sector magic number
dw 0xAA55
