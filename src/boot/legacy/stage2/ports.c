#include "ports.h"

uint8_t port_read(uint16_t port) {
    uint8_t value;
    __asm__("in %%dx, %%al" : "=a" (value) : "d" (port));
    return value;
}

void port_write(uint16_t port, uint8_t value) {
    __asm__("out %%al, %%dx" : : "a" (value), "d" (port));
}
