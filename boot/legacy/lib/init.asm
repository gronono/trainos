init:
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

    ; Setup Stack (goes downward)
    ; so we can call subrutines
    ; To avoid conflict between our code and the stack
    ; setup at the max available
    ; 0x7FFFF = 0x7000 x 0x10 + 0xFFFF
    mov ax, 0x7000
    mov ss, ax
    mov sp, 0xFFFF
    mov bp, sp

    ; Continue from our new location
    ; CS will be 0x0000
    jmp 0x0000:after_move

after_move:
    ; Reset segments
    mov ax, 0
    mov ds, ax  ; Data Segment
    mov es, ax  ; Extra Segment
