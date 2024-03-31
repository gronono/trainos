;
; Create the bootloader inside the Master Boot Record.
; Lookup for bootable partitions
; and boot on user choice
;

; NASM directives
[map all /trainos/build/boot/legacy/mbr.map]
bits 16      ; CPU starts in real mode
org 0x0500   ; Will moved to 0x500 so set origin to that

%include "../lib/symbols.asm"
; Our location after moving
%define ORIGIN_ADDR                  0x0500

begin:
    %include "../lib/init.asm"

    call menu_loop

select:
    mov cx, [selected_menu]
    call is_bootable
    jne .no_bootable

    call load_vbr
    jc .disk_error

    mov si, msg_boot_next
    call print_string
    BREAKPOINT
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
%include "../lib/print_string.asm"
%include "../lib/print_char.asm"
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
times MBR_SIZE - PARTITION_TABLE_SIZE - ( end - begin ) db 0
