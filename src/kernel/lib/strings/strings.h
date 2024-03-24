#pragma once

#include "../varargs.h"
#include "../typedefs.h"

/**
 * Type definition for a function pointer that handles printing a single character.
 *
 * @param character The character to be printed.
 */
typedef void (*PutCharFct)(const char character);

/**
 * Formats and prints data to the output using a custom character output function.
 *
 * This function works similar to the standard printf function, but allows the user to define
 * a custom function for outputting characters.
 *
 * @param put_char_fct The function pointer to the custom character output function.
 * @param format       The format string specifying the format of the output.
 * @param vargs        The variable arguments list containing the data to be printed.
 */
void print(const PutCharFct put_char_fct, const char* format, va_list vargs);