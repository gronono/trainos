section .text
global call_int_15_e820

call_int_15_e820:
    mov eax, 0xE820       ; Fonction E820
    mov ebx, 0   ; Signature 'SMAP'
    mov ecx, 24           ; Taille de la structure à 24 bytes
    mov edx, 0x0534D4150         ; Magic number 0x80
    mov edi, 0x8004       ; Adresse de la structure de données (par exemple 0x9000)
    mov [es:di + 20], dword 1 ; force a valid ACPI 3.X entry
    BREAKPOINT
    int 0x15              ; Appel de l'interruption 0x15
    ret