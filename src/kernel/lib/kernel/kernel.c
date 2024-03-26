#include "kernel.h"
#include "../cpu/interrupt.h"
#include "../uart/uart.h"
#include "../varargs.h"

void kernel_init() {
    uart_init(COM1);
    kprintf("COM1 initialized\n");
    kprintf("Pointer size: %d\n", sizeof(size_t));
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



