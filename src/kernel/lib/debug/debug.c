#include "debug.h"
#include "../uart/uart.h"
#include "../strings/strings.h"

void debug_init() {
    uart_init(COM1);
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
