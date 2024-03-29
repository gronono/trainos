#include <kernel/kernel.h>
#include <memory/memory.h>
#include <hardware/vga_text.h>
#include <hardware/idt.h>
#include <stdio.h>

__attribute__((section(".kmain"))) void kmain() {
    kernel_init();
    vga_text_reset();
    init_idt();
    init_memory();

//    int a = 0;
//    printf("%u", 5 / a);

    // Never returns from bootloader
    kernel_halt();
}