#include "kernel.h"
#include "../hardware/interrupt.h"
#include "../hardware/uart/uart.h"
#include "../hardware/vga/vga_text.h"

int kernel_putc(int ch) {
    uart_write(COM1, ch);
    vga_text_put_char(ch);
    return ch;
}

FILE _stdout = {
    .error = 0,
    .putc = kernel_putc
};
FILE* stdout = &_stdout;
FILE* stderr = &_stdout;

void kernel_init() {
    uart_init(COM1);

    printf("COM1 initialized\n");
    printf("sizeof(char): %u\n", sizeof(char));
    printf("sizeof(int): %u\n", sizeof(int));
    printf("sizeof(uint8_t): %u\n", sizeof(uint8_t));
    printf("sizeof(uint16_t): %u\n", sizeof(uint16_t));
    printf("sizeof(uint32_t): %u\n", sizeof(uint32_t));
    printf("sizeof(uint64_t): %u\n", sizeof(uint64_t));
    printf("sizeof(size_t): %u\n", sizeof(size_t));
    printf("sizeof(uintptr_t): %u\n", sizeof(uintptr_t));
}

void kernel_halt() {
    printf("!! System Halt !!\n");
    interrupt_clear_flag();
    for (;;) {}
}

void kernel_panic(const char* format, ...) {
    fprintf(stderr, "!! Kernel Panic !!\n");

    va_list vargs;
    va_start(vargs, format);
    vfprintf(stderr, format, vargs);
    va_end(vargs);

    kernel_halt();
}
