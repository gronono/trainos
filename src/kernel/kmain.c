#include "lib/debug/debug.h"
#include "lib/console/console.h"
#include "lib/cpu/interrupt.h"
#include "lib/kernel/kernel.h"
#include "lib/vga/vga_text.h"
#include "lib/cpu/bios.h"

__attribute__((section(".kmain"))) void kmain() {
    debug_init();
    vga_text_init();
    console_init(vga_text_put_char);
    console_print("Starting kernel...\n");
    init_idt();
    int a = 5, b = 0;
//    debug("a=%d\n", a / b);

//    debug("Avant call_int_15_e820");
//    BREAKPOINT
//    call_int_15_e820();
//    debug("Apres call_int_15_e820");

    // Never returns from bootloader
    console_print("Halt!\n");
    halt();
}
