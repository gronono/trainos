#include "kernel.h"
#include "lib/debug/debug.h"

void panic(const char* format, ...) {
    debug("Kernel Panic !!!!\n");

    va_list vargs;
    va_start(vargs, format);
    debug_vargs(format, vargs);
    va_end(vargs);

    while (1);
}