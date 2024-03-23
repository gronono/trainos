#include "lib/vga/vga_text.h"
#include "lib/debug/debug.h"
#include "lib/console//console.h"

__attribute__((section(".kmain"))) void kmain() {
    debug_init();
    vga_text_init();
    console_init(vga_text_put_char);

    debug("Starting Kernel...\n");
    console_print("Starting kernel...\n");

    // Never returns from bootloader
    debug("Never end loop\n");
    for (;;) {}
}
