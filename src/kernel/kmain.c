#include "lib/vga/vga_text.h"
#include "lib/debug/debug.h"
#include "lib/console/console.h"
#include "lib/cpu//bios.h"

__attribute__((section(".kmain"))) void kmain() {
    debug_init();
    vga_text_init();
    console_init(vga_text_put_char);
    console_print("Starting kernel...\n");

//    debug("Avant call_int_15_e820");
//    BREAKPOINT
//    call_int_15_e820();
//    debug("Apres call_int_15_e820");

    // TODO  interruptions
    // set_interrupt_flag();

    // Never returns from bootloader
    debug("Kernel started\n");
    for (;;) {}
}
