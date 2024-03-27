#include "lib/cpu/interrupt.h"
#include "lib/kernel/kernel.h"
#include "lib/vga/vga_text.h"
#include "lib/memory/memory.h"

__attribute__((section(".kmain"))) void kmain() {
    vga_text_reset();
    kernel_init();
    init_idt();

    print_memory_map();

    // Never returns from bootloader
    halt();
}
