#include "lib/uart/uart.h"

__attribute__((section(".kmain"))) void kmain() {
    __asm__("xchgw %bx, %bx");

    uart_init_serial(COM1);
    uart_write_serial(COM1, 'H');
    uart_write_serial(COM1, 'e');
    uart_write_serial(COM1, 'l');
    uart_write_serial(COM1, 'l');
    uart_write_serial(COM1, 'o');

    __asm__("xchgw %bx, %bx");

    // Never returns to booloader
    for (;;) {}
}
