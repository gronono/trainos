#pragma once

#include "../varargs.h"

#define BREAKPOINT __asm__ volatile("xchgw %bx, %bx");

/**
 * Initializes the debugging functionality.
 *
 * This function sets up the debugging output to use the UART on COM1.
 * It initializes the UART communication and prepares it for sending debug messages.
 */
void debug_init();

/**
 * Prints a formatted string to the debug output.
 *
 * This function works similar to the standard printf function, but prints the output to
 * the debug output using the UART on COM1.
 *
 * @param format The format string for the debug message.
 * @param ...    Additional variable arguments containing the data to be printed.
 */
void debug(const char* format, ...);

void debug_vargs(const char* format, va_list vargs);
