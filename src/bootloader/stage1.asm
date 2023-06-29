bits 16
org 0x7C00


%define EOL                     0x0D, 0x0A
msg_hello:                  db 'Hello Stage1!', EOL, 0

begin:

%include "print.asm"

end:
; Fill the rest of space with zeros
times 512 - 2 - ( end - begin ) db 0
dw 0AA55h