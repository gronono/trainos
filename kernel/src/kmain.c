#include <kernel/kernel.h>
#include <memory/memory.h>
#include <hardware/vga_text.h>
#include <hardware/idt.h>
#include <stdio.h>

typedef struct KMainArgs {
    uint8_t boot_disk;
    void* memory_map;
} KMainArgs_t;

__attribute__((section(".kmain"))) void kmain(KMainArgs_t args) {
    kernel_init();
    vga_text_reset();
    printf("Disk number: %#x\n", args.boot_disk);
    printf("Memory map address: %p\n", args.memory_map);
    idt_init();

    memory_init(args.memory_map);
    int a = 0;
    printf("%u", 5 / a);

    // Never returns from bootloader
    kernel_halt();
}
