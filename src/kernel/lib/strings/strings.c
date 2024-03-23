#include "strings.h"

void print(const PutCharFct put_char_fct, const char* string) {
    while (*string != '\0') {
        put_char_fct(*string);
        string++;
    }
}