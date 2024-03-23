#pragma once

#include "../typedefs.h"

#define COM1    0x3F8

uint8_t uart_init(uint16_t port);
void uart_write(uint16_t port, uint8_t c);
uint8_t uart_read(uint16_t port);
