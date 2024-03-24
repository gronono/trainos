#pragma once

#include "../typedefs.h"
#include "../strings/strings.h"

/**
 * Initializes the console output functionality with a custom character output function.
 *
 * This function sets up the console output to use a custom function for printing characters.
 *
 * @param put_char_fct The function pointer to the custom character output function.
 */
void console_init(const PutCharFct put_char_fct);

/**
* Prints a formatted string to the console output.
*
* This function works similar to the standard printf function, but prints the output to the console
* using the custom character output function previously initialized with console_init.
*
* @param string The format string specifying the format of the output.
* @param ...    Additional variable arguments containing the data to be printed.
*/
void console_print(const char* string, ...);
