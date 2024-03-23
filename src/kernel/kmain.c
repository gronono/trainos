#include "lib/vga/vga_text.h"
#include "lib/debug/debug.h"
#include "lib/console//console.h"

__attribute__((section(".kmain"))) void kmain() {
    debug_init();
    vga_text_init();
    console_init(vga_text_put_char);

    debug("%s,%s,%c,%c,%d,%d,%u,%u,0x%x,0x%X\n", "hello", NULL, -10, 'A', 10, -5, -2, 5782,42, 42);
    console_print("Starting kernel...\n");

    // Never returns from bootloader
    debug("Never end loop\n");
    for (;;) {}
}
