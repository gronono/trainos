; use the INT 0x15, eax= 0xE820 BIOS function to get a memory map
;
memory_map:
    ; MEMORY_MAP_ADDR is the location of the map
    mov edi, MEMORY_MAP_ADDR + 4    ; destination (+4 = size of uint32 = size of entry count)
    xor ebx, ebx                    ; first entry
    xor ebp, ebp                    ; entry count
    mov edx, 'PAMS'                 ; signature SMAP
    mov eax, 0xE820                 ; function E820
    mov [es:di + 20], dword 1       ; force a valid ACPI 3.X entry
    mov ecx, 24		                ; ask for 24 bytes
    int 0x15                        ; call
    jc .failed                      ; carry set on first call means "unsupported function"
    cmp edx, 'PAMS'     	        ; on success, eax must have been reset to "SMAP"
    jne .failed
    test ebx, ebx                   ; ebx = 0 implies list is only 1 entry long (worthless)
    jz .failed
    jmp .start                      ; ebx != 0, have a valid entry

.next_entry:
    mov edx, 'PAMS'     	        ; restore signature
    mov ecx, 24		                ; ask for 24 bytes
    mov eax, 0xE820                 ; reset function E820
    int 0x15                        ; call next entry

.start:
    jcxz .skip_entry		        ; skip any 0 length entries
    mov ecx, [es:di + 8]	        ; get lower uint32_t of memory region length
    or ecx, [es:di + 12]	        ; "or" it with upper uint32_t to test for zero
    jz .skip_entry

.good_entry:
    inc ebp			                ; got a good entry: ++count, move to next storage spot
    add di, 24

.skip_entry:
    test ebx, ebx		            ; if ebx resets to 0, list is complete
    jz .done
    jmp .next_entry

.failed:
    mov si, msg_smap_failed
    call print_string
    jmp $

.done:
    mov [MEMORY_MAP_ADDR], ebp       ; store the entry count
