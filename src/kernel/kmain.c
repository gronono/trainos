#include "lib/vga/vga_text.h"
#include "lib/debug/debug.h"

__attribute__((section(".kmain"))) void kmain() {
    debug_init();
    vga_text_init();

    debug("Starting Kernel...\n");
    vga_text_put_string("Starting kernel...\n");

    // Never returns from bootloader
    for (;;) {}
}
