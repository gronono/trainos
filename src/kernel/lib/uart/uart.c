#include "uart.h"

static inline unsigned char inb(const uint16_t port) {
    unsigned char data;
    __asm__ volatile("inb %1, %0" : "=a" (data) : "Nd" (port));
    return data;
}

static inline void outb(const uint16_t port, const uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}

bool is_transmit_empty(const uint16_t port) {
    return inb(port + 5) & 0x20;
}

uint8_t received(const uint16_t port) {
    return inb(port + 5) & 1;
}

bool uart_init(const uint16_t port) {
    outb(port + 1, 0x00);    // Disable all interrupts
    outb(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(port + 1, 0x00);    //                  (hi byte)
    outb(port + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(port + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(port + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if (inb(port + 0) != 0xAE) {
        return false;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(port + 4, 0x0F);
    return true;
}

void uart_write(const uint16_t port, uint8_t c) {
    while (is_transmit_empty(port) == 0);

    outb(port,c);
}

uint8_t uart_read(const uint16_t port) {
    while (received(port) == 0);

    return inb(port);
}