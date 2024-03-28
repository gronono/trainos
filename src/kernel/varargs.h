#pragma once

/**
 * Definitions for handling variable argument lists.
 *
 * This header file provides macros and typedefs for working with variable argument lists
 * in functions. It defines the va_list type and macros to initialize, access, and clean up
 * variable argument lists.
 *
 * The following macros are defined:
 *
 * - va_list: A type representing a variable argument list.
 * - va_start: Macro to initialize a va_list for accessing the variable arguments.
 * - va_end: Macro to clean up resources associated with a va_list.
 * - va_arg: Macro to access the next argument from a va_list.
 * - va_copy: Macro to copy the state of a va_list.
 *
 * These macros are typically used in functions that take a variable number of arguments,
 * such as printf. They provide a portable way to access the arguments within the function.
 */

typedef __builtin_va_list   va_list;
#define va_start(v, l)	    __builtin_va_start(v, l)
#define va_end(v)	        __builtin_va_end(v)
#define va_arg(v, T)	    __builtin_va_arg(v, T)
#define va_copy(d, s)	    __builtin_va_copy(d, s)