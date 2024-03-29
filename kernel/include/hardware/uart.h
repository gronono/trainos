#pragma once

#include <stdint.h>

/**
 * The base I/O port address for UART communication on COM1.
 */
#define COM1    0x3F8

/**
 * Initializes the UART communication.
 *
 * This function initializes the UART communication by setting the baud rate,
 * data bits, stop bits, and parity settings. It also configures the communication
 * parameters for proper UART operation.
 *
 * @param port The I/O port address of the UART.
 * @return     STATUS_SUCCESS if initialization was successful, STATUS_FAILURE otherwise.
 */
int uart_init(const uint16_t port);

/**
 * Writes a single byte to the UART.
 *
 * This function writes a single byte to the specified UART port.
 *
 * @param port The I/O port address of the UART.
 * @param c    The byte to be written.
 */
void uart_write(const uint16_t port, uint8_t c);

/**
 * Reads a single byte from the UART.
 *
 * This function reads a single byte from the specified UART port.
 *
 * @param port The I/O port address of the UART.
 * @return     The byte read from the UART.
 */
uint8_t uart_read(const uint16_t port);
