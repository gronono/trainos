#include "debug.h"
#include "../uart/uart.h"

void debug_init() {
    uart_init(COM1);
}

void debug(const char* string) {
    while (*string != '\0') {
        uart_write(COM1, *string);
        string++;
    }
}