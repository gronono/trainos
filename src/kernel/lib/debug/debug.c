#include "debug.h"
#include "../uart/uart.h"
#include "../strings/strings.h";

void debug_init() {
    uart_init(COM1);
}

void write_com1(const char c) {
    uart_write(COM1, c);
}

void debug(const char* string) {
    print(write_com1, string);
}
