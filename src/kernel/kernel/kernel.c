#include "kernel.h"
#include "../hardware/interrupt.h"
#include "../hardware/uart/uart.h"

void kernel_init() {
    uart_init(COM1);
    kprintf("COM1 initialized\n");
    kprintf("sizeof(char): %u\n", sizeof(char));
    kprintf("sizeof(int): %u\n", sizeof(int));
    kprintf("sizeof(uint8_t): %u\n", sizeof(uint8_t));
    kprintf("sizeof(uint16_t): %u\n", sizeof(uint16_t));
    kprintf("sizeof(uint32_t): %u\n", sizeof(uint32_t));
    kprintf("sizeof(uint64_t): %u\n", sizeof(uint64_t));
    kprintf("sizeof(size_t): %u\n", sizeof(size_t));
    kprintf("sizeof(uintptr_t): %u\n", sizeof(uintptr_t));
}

void kernel_halt() {
    kprintf("!! System Halt !!\n");
    interrupt_clear_flag();
    for (;;) {}
}

void kernel_panic(const char* format, ...) {
    kprintf("!! Kernel Panic !!\n");

    va_list vargs;
    va_start(vargs, format);
    kprintf_vargs(format, vargs);
    va_end(vargs);

    kernel_halt();
}



