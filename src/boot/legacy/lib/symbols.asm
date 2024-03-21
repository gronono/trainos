; Our location after moving
%define ORIGIN_ADDR                  0x0500

; Magic breakpoint from Bochs
%define BREAKPOINT                  xchg bx, bx

;
; Some ASCII codes
;
%define CR                          0x0D    ; Carriage Return
%define LF                          0x0A    ; Line Feed
%define EOL                         CR, LF  ; End of Line \r\n

; Partition tables
%define PARTITION_TABLE_OFFSET      0x1BE   ; Offset of the start of partition table
%define PARTITION_ENTRY_SIZE        0x10    ; Size of a partition entry
%define BOOT_FLAG_OFFSET            0x00    ; Boot indicator bit flag: 0 = no, 0x80 = bootable
%define BOOT_FLAG_ENABLED           0x80    ; Value of boot flag when a partition is bootable
%define LBA_ADDR_OFFSET             0x08    ; Offset of the partition LBA

; Some Keyboard controller constants
%define KdbDataPort                 0x60
%define KdbCommandPort              0x64
%define KdbDisable                  0xAD
%define KdbEnable                   0xAE
%define KdbReadPort                 0xD0
%define KdbWritePort                0xD1
; Somes keys
%define UP_ARROW                    0x4800
%define DOWN_ARROW                  0x5000
%define ENTER_KEY                   0x1C0D

;
; GDT tables
;
%define GDT_CODE                    0x08
%define GDT_DATA                    0x10

; Size of FAT headers
; FAT16 = 0x3E
; FAT32 = 0x05A
%define FAT_HEADERS_SIZE            0x3E
%define FAT_HIDDEN_SECTORS_OFFSET   0x1C
