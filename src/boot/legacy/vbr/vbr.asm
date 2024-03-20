;
; Create the booloader inside the Volume Boot Record.
; The main task is to locate the kernel and jump to it.
;

; NASM directives
[map all /trainos/build/boot/legacy/mbr.map]
bits 16 ; CPU starts in real mode
org 62   ; Set origin after FAT16 headers

%include "../lib/symbols.asm"

begin:
    %include "../lib/init.asm"

    mov si, msg_welcome
    call print_string
    ; just in case. Should not be call
    hlt

%include "../lib/print_string.asm"
msg_welcome     db 'TrainOS VBR', EOL, EOL, 0

end:
; Fill the rest of space with zeros
times 448 - ( end - begin ) db 0
