#include <kernel/kernel.h>
#include <hardware/interrupt.h>
#include <stdio.h>

void kernel_panic(const char* format, ...) {
    fprintf(stderr, "!! Kernel Panic !!\n");

    va_list vargs;
    va_start(vargs, format);
    vfprintf(stderr, format, vargs);
    va_end(vargs);

    kernel_halt();
}
