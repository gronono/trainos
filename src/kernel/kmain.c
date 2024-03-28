#include "kernel/kernel.h"
#include "memory/memory.h"
#include "hardware/interrupt.h"
#include "hardware/vga/vga_text.h"

__attribute__((section(".kmain"))) void kmain() {
    vga_text_reset();
    kernel_init();
    init_idt();

    print_memory_map();
    int a = 0;
    kprintf("%u", 5 / a);

    // Never returns from bootloader
    kernel_halt();
}
