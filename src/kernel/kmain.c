#include "kernel/kernel.h"
#include "memory/memory.h"
#include "hardware/interrupt.h"
#include "hardware/vga/vga_text.h"
#include "hardware/idt/idt.h"

__attribute__((section(".kmain"))) void kmain() {
    kernel_init();
    vga_text_reset();
    init_idt();
    print_memory_map();
//    int a = 0;
//    printf("%u", 5 / a);

    // Never returns from bootloader
    kernel_halt();
}
