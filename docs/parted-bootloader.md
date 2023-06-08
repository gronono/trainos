# Understanding bootstrap code from parted

When created a MBR partition table, `parted` generate some bootstrap code.

Here an exemple:

```bash
$ dd if=/dev/zero of=disk.bin bs=1M count=128 
128+0 enregistrements lus
128+0 enregistrements écrits
134217728 octets (134 MB, 128 MiB) copiés, 0,120925 s, 1,1 GB/s
$ parted -s disk.bin mktable msdos
$ hexdump -C disk.bin
00000000  fa b8 00 10 8e d0 bc 00  b0 b8 00 00 8e d8 8e c0  |................|
00000010  fb be 00 7c bf 00 06 b9  00 02 f3 a4 ea 21 06 00  |...|.........!..|
00000020  00 be be 07 38 04 75 0b  83 c6 10 81 fe fe 07 75  |....8.u........u|
00000030  f3 eb 16 b4 02 b0 01 bb  00 7c b2 80 8a 74 01 8b  |.........|...t..|
00000040  4c 02 cd 13 ea 00 7c 00  00 eb fe 00 00 00 00 00  |L.....|.........|
00000050  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
000001b0  00 00 00 00 00 00 00 00  04 f8 22 92 00 00 00 00  |..........".....|
000001c0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
000001f0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 55 aa  |..............U.|
00000200  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
08000000
```

I used an [online tool](https://shell-storm.org/online/Online-Assembler-and-Disassembler/?opcodes=fa+b8+00+10+8e+d0+bc+00++b0+b8+00+00+8e+d8+8e+c0fb+be+00+7c+bf+00+06+b9++00+02+f3+a4+ea+21+06+0000+be+be+07+38+04+75+0b++83+c6+10+81+fe+fe+07+75f3+eb+16+b4+02+b0+01+bb++00+7c+b2+80+8a+74+01+8b4c+02+cd+13+ea+00+7c+00++00+eb+fe+&arch=x86-16&endianness=little&baddr=0x00000000&dis_with_addr=True&dis_with_raw=True&dis_with_ins=True#disassembly) to disassemble the machine code. And here the resulting assembler code with my own comments:
```asm
; Disable interruptions
0x00:  FA                cli       

;
; Setup stack
; because segment registers cannot be set directly, we use the AX register
0x01:  B8 00 10          mov       ax, 0x1000
; Stack Segment = 0x1000
0x04:  8E D0             mov       ss, ax
; Stack Pointer = 0xb000
0x06:  BC 00 B0          mov       sp, 0xb000

; Reset Data Segment and Extra Segment to 0
0x09:  B8 00 00          mov       ax, 0
0x0c:  8E D8             mov       ds, ax
0x0e:  8E C0             mov       es, ax

; Enable interrupts
0x10:  FB                sti    

;
; Copy our code from 0x7C00 to 0x600
;
; Source = 0x7C00 (BIOS loads boot sector into this address)
0x11:  BE 00 7C          mov       si, 0x7c00
; Destination = 0x600
0x14:  BF 00 06          mov       di, 0x600
; Counter = 0x200 = 512 bytes = 1 sector
0x17:  B9 00 02          mov       cx, 0x200
; Replicates CX times = mov byte from DS:SI to ES:DI; SI++; DI++
; so copy 512 bytes from 07C00 to 0x600
0x1a:  F3 A4             rep movsb byte ptr es:[di], byte ptr [si]

; Jump to the new location (0x600 + 21)
; resume to the next line
0x1c:  EA 21 06 00 00    ljmp      0:0x621

;
; Look for each partition
; and test if the boot flag is set
; if so load the boot sector of the partition
; if no partition is bootable, 
;
; 0x7BE = 0x600 + 0x1BE
; and 0x1BE is the boot flag for the first partition
0x21:  BE BE 07          mov       si, 0x7be

; Begin of the loop for each partition
; AX = 0, so AL = 0
; compare the boot flag of the first partition to 0
0x24:  38 04             cmp       byte ptr [si], al
; if boot flag is set (double negation), jump to the code that load the boot sector of this partition
0x26:  75 0B             jne       0x33
; the boot flag of the first partition is not set
; SI = SI + 16 so SI point to the next partition entry
0x28:  83 C6 10          add       si, 0x10
; 0x7FE = 0x600 + 0x1FE
; and 0x1FE is the boot signature
0x2b:  81 FE FE 07       cmp       si, 0x7fe
; if SI != boot signature, continue the loop
0x2f:  75 F3             jne       0x24
; else goto the infinite loop
0x31:  EB 16             jmp       0x49

;
; Load the boot sector inside the partition
;
; AH = 2 => reading a sector
0x33:  B4 02             mov       ah, 2
; AL = 1 => reading a single sector
0x35:  B0 01             mov       al, 1
; BX = 0x7C00 => writing to 0x7C00
; 0x7C00 is the standard boot code address
0x37:  BB 00 7C          mov       bx, 0x7c00
; use 0x80 drive = "C:" drive = first drive
0x3a:  B2 80             mov       dl, 0x80
; SI + 1 / SI + 2 / SI + 3 = CHS address of the start of the partition
; so DH is the head of this addess
0x3c:  8A 74 01          mov       dh, byte ptr [si + 1]
; and CS is CH/CL so the cylinder and sector parts of the CHS address 
0x3f:  8B 4C 02          mov       cx, word ptr [si + 2]
; invoke the reading
0x42:  CD 13             int       0x13
; Jump to the new code sector
0x44:  EA 00 7C 00 00    ljmp      0:0x7c00

; Infinite loop
0x49:  EB FE             jmp       0x49
```
