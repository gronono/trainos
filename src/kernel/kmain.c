#include "lib/uart/uart.h"

__attribute__((section(".kmain"))) void kmain() {
    __asm__("xchgw %bx, %bx");

    uart_init(COM1);
    uart_write(COM1, 'H');
    uart_write(COM1, 'e');
    uart_write(COM1, 'l');
    uart_write(COM1, 'l');
    uart_write(COM1, 'o');

    __asm__("xchgw %bx, %bx");

    // Never returns to booloader
    for (;;) {}
}
