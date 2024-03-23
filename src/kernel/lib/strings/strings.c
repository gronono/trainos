#include "strings.h"

void print(const PutCharFct put_char_fct, const char * string, va_list vargs);

#define FLAG_SIGN       0b01
#define FLAG_UPPERCASE  0b10

char lower_digits[] = "0123456789abcdef";
char upper_digits[] = "0123456789ABCDEF";

void handle_integer(const PutCharFct put_char_fct, const int value, const uint8_t base, uint8_t flags) {
    uint32_t uvalue;
    bool sign = false;
    if ((flags & FLAG_SIGN) && value < 0) {
        sign = true;
        uvalue = -value;
    } else {
        sign = false;
        uvalue = value;
    }

    char* digits = lower_digits;
    if (flags & FLAG_UPPERCASE) {
        digits = upper_digits;
    }

    int i = 0;
    char buffer[16];
    do {
        buffer[i++] = digits[uvalue % base];
        uvalue /= base;
    } while (uvalue != 0);

    if (sign) {
        buffer[i++] = '-';
    }

    while (i > 0) {
        put_char_fct(buffer[--i]);
    }
}

void handle_pointer(const PutCharFct put_char_fct, const ptr_t pointer, const uint8_t flags) {
    put_char_fct('0');
    put_char_fct('x');

    char* digits = lower_digits;
    if (flags & FLAG_UPPERCASE) {
        digits = upper_digits;
    }

    uint32_t value = pointer;
    uint8_t size = sizeof(pointer);
    for (uint8_t i = 0; i < (size * 2); i++) {
        put_char_fct(digits[value >> (size * 8 - 4)]);
        value = value << 4;
    }
}

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

void print(const PutCharFct put_char_fct, const char * format, va_list vargs) {
    char c;
    for (uint8_t i = 0; (c = format[i]) != '\0'; i++) {
        if (c != '%') {
            // standard char
            put_char_fct(c);
            continue;
        }
        // Found a formatted part
        c = format[++i];
        if (c == 0) {
            // % is the last char of string
            put_char_fct('%');
            break;
        }
        switch (c) {
            case 'c':
                put_char_fct(va_arg(vargs, int));
                break;
            case 'd':
            case 'i':
                handle_integer(put_char_fct, va_arg(vargs, int), 10, FLAG_SIGN);
                break;
            case 'o':
                handle_integer(put_char_fct, va_arg(vargs, int), 8, 0);
                break;
            case 'p':
                handle_pointer(put_char_fct, va_arg(vargs, ptr_t), 0);
                break;
            case 'P':
                handle_pointer(put_char_fct, va_arg(vargs, ptr_t), FLAG_UPPERCASE);
                break;
            case 's':
                handle_string(put_char_fct, va_arg(vargs, char *));
                break;
            case 'u':
                handle_integer(put_char_fct, va_arg(vargs, int), 10, 0);
                break;
            case 'x':
                handle_integer(put_char_fct, va_arg(vargs, int), 16, 0);
                break;
            case 'X':
                handle_integer(put_char_fct, va_arg(vargs, int), 16, FLAG_UPPERCASE);
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