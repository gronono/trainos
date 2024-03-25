; use the INT 0x15, eax= 0xE820 BIOS function to get a memory map
;
memory_map:
    ; SMAP_ADDR is the location of the map
    ; the first byte is the number of entries
    mov ebp, 0                  ; entry count
    mov ebx, 0                  ; first entry
    mov edi, SMAP_ADDR + 1      ; destination
    mov edx, 'PAMS'             ; signature SMAP
    mov eax, 0xE820             ; function E820
    mov [es:di + 20], dword 1   ; force a valid ACPI 3.X entry
    mov ecx, 24		            ; ask for 24 bytes
    int 0x15                    ; call
    jc .failed                  ; carry set on first call means "unsupported function"
    mov edx, 'PAMS'     	    ; some BIOSes apparently trash this register
    cmp eax, edx		        ; on success, eax must have been reset to "SMAP"
    jne .failed
    test ebx, ebx               ; ebx = 0 implies list is only 1 entry long (worthless)
    je .failed
    jmp .start                  ; ebx != 0, have a valid entry

.next_entry:
    mov eax, 0xE820             ; reset function E820
    mov [es:di + 20], dword 1   ; force a valid ACPI 3.X entry
    mov ecx, 24		            ; ask for 24 bytes
    int 0x15                    ; call next entry
    jc .done
    mov edx, 'PAMS'     	    ; restore signature

.start:
    jcxz .skip_entry		    ; skip any 0 length entries
    cmp cl, 0x14		        ; got a 24 byte ACPI 3.X response?
    jbe .good_entry
    test byte [es:di + 20], 1	; if so: is the "ignore this data" bit clear?
    je short .skip_entry

.good_entry:
    mov ecx, [es:di + 8]	    ; get lower uint32_t of memory region length
    or ecx, [es:di + 12]	    ; "or" it with upper uint32_t to test for zero
    jz .skip_entry		        ; if length uint64_t is 0, skip entry
    inc bp			            ; got a good entry: ++count, move to next storage spot
    add di, 24

.skip_entry:
    test ebx, ebx		        ; if ebx resets to 0, list is complete
    jne short .next_entry
    jmp .done

.failed:
    mov si, msg_smap_failed
    call print_string
    jmp $

.done:
    mov [SMAP_ADDR], bp         ; store the entry count


