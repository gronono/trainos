#include "strings.h"

void print(const PutCharFct put_char_fct, const char * string, va_list vargs);

void handle_string(const PutCharFct put_char_fct, const char * value) {
    if (value == NULL) {
        print(put_char_fct, "(null)", NULL);
    } else {
        while (*value != '\0') {
            put_char_fct(*value);
            value++;
        }
    }
}

void print(const PutCharFct put_char_fct, const char * string, va_list vargs) {
    if (string == NULL) {
        // TODO Panic
        return;
    }

    char c;
    for (int i = 0; (c = string[i]) != '\0'; i++) {
        if (c != '%') {
            // standard char
            put_char_fct(c);
            continue;
        }
        // Found a formatted part
        c = string[++i];
        if (c == 0) {
            // % is the last char of string
            break;
        }
        switch (c) {
            case 's':
                handle_string(put_char_fct, va_arg(vargs, char *));
                break;
            case '%':
                put_char_fct('%');
                break;
            default:
                // unknown % flag
                put_char_fct('%');
                put_char_fct(c);
                break;
        }

    }
}