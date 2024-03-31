ENTRY(kmain)
OUTPUT_FORMAT("binary")

SECTIONS
{
    /*
     * Just after the VBR located at 0x500 + 1 sector (=512=0x200) = 0x700
     */
    . = 0x08E2;

    PROVIDE(kernel_start = .);

    /* The kmain function must be the first thing as required by the VBR  */
    .kmain : {
        *(.kmain)
    }

    /** text = code C */
    .text : {
        *(.text)
    }

    /** data = initialized data */
    .data : {
        *(.data)
    }

    /** bss = uninitialized data */
    .bss : {
        *(.bss)
    }

    PROVIDE(kernel_end = .);
}
