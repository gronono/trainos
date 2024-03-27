#include "kernel.h"
#include "../cpu/interrupt.h"
#include "../uart/uart.h"
#include "../varargs.h"

void kernel_init() {
    uart_init(COM1);
    kprintf("COM1 initialized\n");
    kprintf("sizeof(uint8_t): %d\n", sizeof(uint8_t));
    kprintf("sizeof(uint16_t): %d\n", sizeof(uint16_t));
    kprintf("sizeof(uint32_t): %d\n", sizeof(uint32_t));
    kprintf("sizeof(uint64_t): %d\n", sizeof(uint64_t));
    kprintf("sizeof(size_t): %d\n", sizeof(size_t));
    kprintf("sizeof(uintptr_t): %d\n", sizeof(uintptr_t));
}

void halt() {
    kprintf("!! System Halt !!\n");
    clear_interrupt_flag();
    for (;;) {}
}

void panic(const char* format, ...) {
    kprintf("!! Kernel Panic !!\n");

    va_list vargs;
    va_start(vargs, format);
    kprintf(format, vargs);
    va_end(vargs);

    halt();
}



