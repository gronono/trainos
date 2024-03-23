#pragma once

#include "../varargs.h"
#include "../typedefs.h"

typedef void (*PutCharFct)(const char character);

void print(const PutCharFct put_char_fct, const char* format, va_list vargs);