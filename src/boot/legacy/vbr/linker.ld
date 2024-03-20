ENTRY(entry)
OUTPUT_FORMAT("binary")
phys = 0x503E;  # Skip FAT16 headers (0x500 + 0x3E)

SECTIONS
{
    . = phys;

    .entry  : { __entry_start  = .; *(.entry)  }
    .text   : { __text_start   = .; *(.text)   }
    .data   : { __data_start   = .; *(.data)   }
    .rodata : { __rodata_start = .; *(.rodata) }
    .bss    : { __bss_start    = .; *(.bss)    }
    __end = .;
}
