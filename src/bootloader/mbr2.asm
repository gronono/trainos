; The second part of MBR located at 0x00E0-0x01B8

bits 16
; The second part size
%define OUR_SIZE            216

begin:

end:
; Fill the rest of space with zeros
times OUR_SIZE - ( end - begin ) db 0