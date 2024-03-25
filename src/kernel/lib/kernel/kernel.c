#include "kernel.h"
#include "../debug/debug.h"
#include "../cpu/interrupt/interrupt.h"

void halt() {
    debug("!! System Halt !!\n");
    __asm__ volatile ("hlt");
    clear_interrupt_flag();
    for (;;) {}
}

void panic(const char* format, ...) {
    debug("!! Kernel Panic !!\n");

    va_list vargs;
    va_start(vargs, format);
    debug_vargs(format, vargs);
    va_end(vargs);

    halt();
}

