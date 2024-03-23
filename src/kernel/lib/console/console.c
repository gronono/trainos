#include "console.h"

static PutCharFct console_put_char_fct;

void console_init(const PutCharFct put_char_fct) {
    console_put_char_fct = put_char_fct;
}

void console_print(const char* format, ...) {
    va_list vargs;
    va_start(vargs, format);
    print(console_put_char_fct, format, vargs);
    va_end(vargs);
}