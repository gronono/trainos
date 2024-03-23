#include "console.h"

static PutCharFct console_put_char_fct;

void console_init(const PutCharFct put_char_fct) {
    console_put_char_fct = put_char_fct;
}

void console_print(const char* string) {
    print(console_put_char_fct, string);
}