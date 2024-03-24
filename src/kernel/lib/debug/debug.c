#include "debug.h"
#include "../uart/uart.h"
#include "../strings/strings.h"

void debug_init() {
    uart_init(COM1);
    debug("COM1 initialized\n");
    debug("Pointer size: %d\n", sizeof(ptr_t));
}

void write_com1(const char c) {
    uart_write(COM1, c);
}

void debug(const char* format, ...) {
    va_list vargs;
    va_start(vargs, format);
    print(write_com1, format, vargs);
    va_end(vargs);
}

void debug_vargs(const char* format, va_list vargs) {
    print(write_com1, format, vargs);
}
