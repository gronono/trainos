;
; Create the bootloader inside the Master Boot Record.
; Lookup for bootable partitions
; and boot to user choose
;

; NASM directives
[map all /trainos/build/boot/legacy/mbr.map]
bits 16 ; CPU starts in real mode
org 0   ; Do not translate addresses

%include "./symbols.asm"

begin:
    ; Disable interrupts
    cli
    ; Reset direction flag, so we know his value
    cld

    ; BIOS put us at 0x0000:0x7C00
    ; Copy our self to the begin of unused RAMM : 0x0500:0
    ; movsb copy from DS:SI to ES:DI
    ; set DS from our source
    mov ax, 0x07C0  ; 0x07C0:0 = 0x07C0 * 0x10 + 0
    mov ds, ax
    ; set es to our destination
    mov ax, 0x0050  ; 0x0500:0 = 0x0050 * 0x10 + 0
    mov es, ax
    ; and set si and di offsets to 0
    mov si, 0
    mov di, 0
    ; counter = 512 bytes = our size
    mov cx, 512
    ; repeat cx time: copy 1 byte from ds:si to es:di and increment si, di by one
    rep movsb

    ; Continue from our new location
    ; CS will be 0x0050
    jmp 0x0050:start

start:
    ; Setup Data-Segment
    ; Because we move to 0x0500, data will be at 0x0050:offset
    mov ax, 0x0050
    mov ds, ax

    ; Setup Stack (goes downward)
    ; so we can call subrutines
    ; To avoid conflict between our code and the stack
    ; setup at the max available
    ; 0x7FFFF = 0x7000 x 0x10 + 0xFFFF
    mov ax, 0x7000
    mov ss, ax
    mov sp, 0xFFFF
    mov bp, sp

    call menu_loop

    ; just in case. Should not be call
    hlt

select:
    mov cx, [selected_menu]
    call is_bootable
    jne .no_bootable

    call load_vbr
    jc .disk_error

    mov si, msg_boot_next
    call print_string
    jmp 0x0000:0x7C00   ; jump to the VBR

    .disk_error:
        mov si, msg_disk_error
        call print_string
     hlt
    .no_bootable:
        call clear_screen
        mov si, msg_no_bootable
        call print_string
        call wait_key
        jmp menu_loop

%include "./menu.asm"
%include "./keyboard.asm"
%include "./print.asm"
%include "./screen.asm"
%include "./disk.asm"

msg_welcome     db 'TrainOS MBR', EOL, EOL, 0
msg_partition   db ' Partition #', 0
msg_no_bootable db 'No bootable partition', EOL, 0
msg_disk_error  db 'Disk Error!', EOL, 0
msg_boot_next   db 'Volume Boot Record...', EOL, 0
msg_eol         db EOL, 0
msg_crlf        db CR, LF, 0
selected_menu   db 0

end:
; Fill the rest of space with zeros
times 440 - ( end - begin ) db 0
