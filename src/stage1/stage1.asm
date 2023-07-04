;
; Create the bootloader inside the Master Boot Record.
; Load and execute stage2
;

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
    
    ; Copy our self 0x0600
    ; movsb copy from DS:SI to ES:DI
    ; set DS from our source
    mov ax, 0x07C0  ; 0x07C0:0 = 0x07C0 * 0x10 + 0 = 0x7C00
    mov ds, ax
    ; set es to our destination
    mov ax, 0x0060 
    mov es, ax
    ; and set si and di offsets to 0
    mov si, 0
    mov di, 0
    ; counter = 512 bytes = our size
    mov cx, 512     
    ; repeat cx time: copy 1 byte from ds:si to es:di and increment si, di by one
    rep movsb       

    ; Continue from our new location
    jmp 0x0060:start

start:
    ; because we move to 0x0600, DS should be 0x60
    mov ax, 0x0060
    mov ds, ax

    ; Setup the stack juste before us
    ; 0x50:0xFF = 0x05FF
    mov sp, 0x00FF
    mov ax, 0x0050
    mov ss, ax

    ; Reset direction flag, so we know this value
    cld
    
    ; Setup finished we enable interruption
    sti
    
    ; Load stage 2
    ; Fill DAP
    mov ax, [stage2_size]
    mov word [disk_dap.number_of_sectors], ax
    ; destination = 0:0x800 (0x600+512)
    mov word [disk_dap.buffer_segment], 0
    mov word [disk_dap.buffer_offset], 0x800
    ; LBA of the partition = 2nd sector = 1
    mov eax, 1
    mov dword [disk_dap.lba], eax
    ; Source = DS:SI = Addres of DAP
    ; DS is already set at 0x60
    mov si, disk_dap

    ; Call read from LBA
    mov ah, 0x42
    int 0x13
    jc disk_error

    ;
    ; Switch to protected mode
    ;
    BREAKPOINT
    call enable_a20
    
    
    ; Jump to stage 2 at 0x0800
    mov ax, 0x80
    mov ds, ax
    mov es, ax
    jmp 0x80:0

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
    sti
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
times 440 - 2 - ( end - begin ) db 0
; (reserve 2 bytes for stage2 length)
stage2_size:                