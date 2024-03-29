#pragma once

#include "vararg.h"

/**
 * The <stdio.h> header provides generic file operation support
 * and supplies functions with narrow character input/output capabilities.
 *
 * https://en.cppreference.com/w/c/io
 */

/**
 * Object type, capable of holding all information needed to control a C I/O stream.
 *
 * https://en.cppreference.com/w/c/io/FILE
 */
typedef struct {
    int error;
    int (*putc)(int ch);
    int (*getc);
} FILE ;

/**
 * https://en.cppreference.com/w/c/io/std_streams
 */
extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

#define EOF -1

/**
 * https://en.cppreference.com/w/c/io/fputc
 */
int fputc(int ch, FILE* stream);
int putc(int ch, FILE* stream);

/**
 *  https://en.cppreference.com/w/c/io/ferror
 */
int ferror(FILE* stream);

/**
 * https://en.cppreference.com/w/c/io/fprintf
 */
int fprintf(FILE* stream, const char* format, ...);
int printf(const char* format, ...);

/**
 * https://en.cppreference.com/w/c/io/vfprintf
 */
int vfprintf(FILE* stream, const char* format, va_list vargs);
int vprintf(const char* format, va_list vargs);
