#include "uart.h"
#include "../port/port.h"

bool is_transmit_empty(const uint16_t port) {
    return port_read(port + 5) & 0x20;
}

uint8_t received(const uint16_t port) {
    return port_read(port + 5) & 1;
}

bool uart_init(const uint16_t port) {
    port_write(port + 1, 0x00);    // Disable all interrupts
    port_write(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    port_write(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    port_write(port + 1, 0x00);    //                  (hi byte)
    port_write(port + 3, 0x03);    // 8 bits, no parity, one stop bit
    port_write(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    port_write(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    port_write(port + 4, 0x1E);    // Set in loopback mode, test the serial chip
    port_write(port + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if (port_read(port + 0) != 0xAE) {
        return false;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    port_write(port + 4, 0x0F);
    return true;
}

void uart_write(const uint16_t port, uint8_t c) {
    while (is_transmit_empty(port) == 0);

    port_write(port,c);
}

uint8_t uart_read(const uint16_t port) {
    while (received(port) == 0);

    return port_read(port);
}