#pragma once

/**
 * https://en.cppreference.com/w/c/variadic
 */

/** https://en.cppreference.com/w/c/variadic/va_list */
typedef __builtin_va_list   va_list;
/** https://en.cppreference.com/w/c/variadic/va_start */
#define va_start(v, l)	    __builtin_va_start(v, l)
/** https://en.cppreference.com/w/c/variadic/va_end */
#define va_end(v)	        __builtin_va_end(v)
/** https://en.cppreference.com/w/c/variadic/va_arg */
#define va_arg(v, T)	    __builtin_va_arg(v, T)
/** https://en.cppreference.com/w/c/variadic/va_copy */
#define va_copy(d, s)	    __builtin_va_copy(d, s)
