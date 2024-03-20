; Carriage Return
%define CR  0x0D
; Line Feed
%define LF  0x0A
; End of Line \r\n
%define EOL                         CR, LF
; Magic breakpoint from Bochs
%define BREAKPOINT                  xchg bx, bx

%define UP_ARROW   0x4800
%define DOWN_ARROW 0x5000
%define ENTER_KEY  0x1C0D

; Size of a partition entry
%define PARTITION_ENTRY_SIZE    0x10
; First address of partition boot flag
%define FIRST_BOOT_FLAG_ADDR    0x1BE
; Value of boot flag when a partition is bootable
%define BOOT_FLAG_ENABLED       0x80
; OFFSET of the LBA addr from partition entry start
%define LBA_ADDR_OFFSET         0x08
