#include <kernel/kernel.h>
#include <hardware/interrupt.h>
#include <hardware/uart.h>
#include <hardware/vga_text.h>

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
    printf("sizeof(char):      %lu\n", sizeof(char));
    printf("sizeof(short):     %lu\n", sizeof(short));
    printf("sizeof(int):       %lu\n", sizeof(int));
    printf("sizeof(uint8_t):   %lu\n", sizeof(uint8_t));
    printf("sizeof(uint16_t):  %lu\n", sizeof(uint16_t));
    printf("sizeof(uint32_t):  %lu\n", sizeof(uint32_t));
    printf("sizeof(uint64_t):  %lu\n", sizeof(uint64_t));
    printf("sizeof(size_t):    %lu\n", sizeof(size_t));
    printf("sizeof(uintptr_t): %lu\n", sizeof(uintptr_t));

    // using char* for subtracting into bytes
    char* kernel_start_addr = (char*) &kernel_start;
    char* kernel_end_addr = (char*) &kernel_end;
    uint32_t kernel_size = kernel_end_addr - kernel_start_addr;
    printf("Kernel is located from %p to %p: %u bytes\n",
           (void*) kernel_start_addr,
           (void*) kernel_end_addr,
           kernel_size);

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
