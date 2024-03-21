ENTRY(entry)
OUTPUT_FORMAT("binary")

SECTIONS
{
    /*
     * Because we relocate us at 0x0500 (see boot.asm)
     * but we have to skip FAT16 headers (0x3E)
     * the physical starting address is 0x500 + 0x3E
     */
    . = 0x053E;

    /* boot.asm */
    .boot : {
        *(.boot)
    }

    /* start C function */
    .start : {
        *(.start)
    }

    /** text = code C */
    .text : {
        *(.text)
    }

    /** data = initialized data */
    .data : {
        *(.data)
    }

    /** bss = initialized data */
    .bss : {
        *(.bss)
    }
}
