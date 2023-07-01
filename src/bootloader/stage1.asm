bits 16
org 0x7C00

begin:

%define EOL                     0x0D, 0x0A
msg_hello:                  db 'Hello Stage1!', EOL, 0


end:
; Fill the rest of space with zeros
times 448 - ( end - begin ) nop