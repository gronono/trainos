#include "port.h"

inline uint8_t port_read(const uint16_t port) {
    uint8_t value;
    __asm__("in %%dx, %%al" : "=a" (value) : "d" (port));
    return value;
}

inline void port_write(const uint16_t port, const uint8_t value) {
    __asm__("out %%al, %%dx" : : "a" (value), "d" (port));
}
