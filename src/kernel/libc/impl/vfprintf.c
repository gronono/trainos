#include "../stdbool.h"
#include "../stddef.h"
#include "../stdio.h"
#include "../string.h"

typedef struct {
    uint8_t state;
    uint8_t flags;
    int width;
    int precision;
    uint8_t length;
    uint8_t type;
    uint8_t extra;
    int written;
    FILE* stream;
} Params;

// Using a state machine
#define STATE_START          0
#define STATE_FLAGS          1
#define STATE_WIDTH          2
#define STATE_PRECISION      3
#define STATE_LENGTH         4
#define STATE_TYPE           5
#define STATE_PRINT          6

// FLAGS = 0 = undefined
#define FLAG_MINUS           0b000001    // unsupported
#define FLAG_PLUS            0b000010
#define FLAG_SPACE           0b000100
#define FLAG_ZERO            0b001000
#define FLAG_APOSTROPHE      0b010000    // unsupported
#define FLAG_HASH            0b100000

// LENGTH = 0 = undefined
#define LENGTH_INT_CHAR      1
#define LENGTH_INT_SHORT     2
#define LENGTH_LONG          3
#define LENGTH_LONG_LONG     4
#define LENGTH_LONG_DOUBLE   5   // unsupported
#define LENGTH_SIZE_T        6
#define LENGTH_INT_MAX_T     7
#define LENGTH_PTR_DIFF_T    8

// Types
#define TYPE_PERCENT         0
#define TYPE_SIGNED_INT      1
#define TYPE_UNSIGNED_INT    2
#define TYPE_DOUBLE_FIXED    3   // unsupported
#define TYPE_DOUBLE_STD      4   // unsupported
#define TYPE_DOUBLE_EXP      5   // unsupported
#define TYPE_UNSIGNED_HEX    6
#define TYPE_UNSIGNED_OCT    7
#define TYPE_STRING          8
#define TYPE_CHAR            9
#define TYPE_POINTER         10
#define TYPE_DOUBLE_HEX      11  // unsupported
#define TYPE_NOTHING         12

#define EXTRA_TYPE_UPPER     0b001
#define EXTRA_WIDTH_STAR     0b010
#define EXTRA_PRECISION_STAR 0b100

void reset(Params* params) {
    params->flags = 0;
    params->width = 0;
    params->precision = 0;
    params->length = 0;
    params->type = 0;
    params->extra = 0;
}


void pre_append(Params* params, char* str, va_list* vargs) {
    if (params->flags & FLAG_MINUS) {
        return;
    }
    uint8_t width = params->width;
    if (params->extra & EXTRA_PRECISION_STAR) {
        width = va_arg(*vargs, int);
    }
    char c = ' ';
    bool numeric = params->type != TYPE_STRING && params->type != TYPE_CHAR;
    if (numeric && params->flags & FLAG_ZERO) {
        c = '0';
    }

    size_t size = strlen(str);
    while (width > size) {
        fputc(c, params->stream);
        width--;
    }
}

void writes(Params* params, char* str, va_list* vargs) {
    pre_append(params, str, vargs);

    for (size_t i = 0; *str != '\0'; i++, str++) {
        if (params->precision == 0 || i < params->precision) {
            fputc(*str, params->stream);
        }
    }
}

void parse_start(Params* params, char** ptr) {
    reset(params);
    if (**ptr == '%') {
        params->state = STATE_FLAGS;
    } else {
        fputc(**ptr, params->stream);
    }
    (*ptr)++;
}

void parse_flags(Params* params, char** ptr) {
    switch (**ptr) {
        case '-':
            params->flags |= FLAG_MINUS;
            (*ptr)++;
            break;
        case '\'':
            params->flags |= FLAG_APOSTROPHE;
            (*ptr)++;
            break;
        case '#':
            params->flags |= FLAG_HASH;
            (*ptr)++;
            break;
        case '+':
            params->flags |= FLAG_PLUS;
            (*ptr)++;
            break;
        case ' ':
            params->flags |= FLAG_SPACE;
            (*ptr)++;
            break;
        case '0':
            params->flags |= FLAG_ZERO;
            (*ptr)++;
            break;
        default:
            params->state = STATE_WIDTH;
    }
}

void parse_width(Params* params, char** ptr) {
    if (**ptr >= '0' && **ptr <= '9') {
        params->width = params->width * 10 + (**ptr - '0');
        (*ptr)++;
    } else if (**ptr == '*') {
        if (params->width == 0) {
            params->extra |= EXTRA_WIDTH_STAR;
        } else {
            fprintf(stderr, "<invalid width '%u*'>", params->width);
        }
        (*ptr)++;
    } else if (**ptr == '.') {
        params->state = STATE_PRECISION;
        (*ptr)++;
    } else {
        params->state = STATE_LENGTH;
    }
}

void parse_precision(Params* params, char** ptr) {
    if (**ptr >= '0' && **ptr <= '9') {
        params->precision = params->precision * 10 + (**ptr - '0');
        (*ptr)++;
    } else if (**ptr == '*') {
        if (params->precision == 0) {
            params->extra |= EXTRA_PRECISION_STAR;
        } else {
            fprintf(stderr,  "<invalid precision '%u*'>", params->precision);
        }
        (*ptr)++;
    } else {
        params->state = STATE_LENGTH;
    }
}

void parse_length(Params* params, char** ptr) {
    switch (**ptr) {
        case 'h':
            if (params->length == LENGTH_INT_SHORT) {
                params->length = LENGTH_INT_CHAR;
            } else {
                params->length = LENGTH_INT_SHORT;
            }
            (*ptr)++;
            break;
        case 'l':
            if (params->length == LENGTH_LONG) {
                params->length = LENGTH_LONG_LONG;
            } else {
                params->length = LENGTH_LONG;
            }
            (*ptr)++;
            break;
        case 'z':
            params->length = LENGTH_SIZE_T;
            (*ptr)++;
            break;
        case 'L':
            params->length = LENGTH_LONG_DOUBLE;
            (*ptr)++;
            break;
        case 'j':
            params->length = LENGTH_INT_MAX_T;
            (*ptr)++;
            break;
        case 't':
            params->length = LENGTH_PTR_DIFF_T;
            (*ptr)++;
            break;
        default:
            params->state = STATE_TYPE;
    }
}

void parse_type(Params* params, char** ptr) {
    switch (**ptr) {
        case '%':
            params->type = TYPE_PERCENT;
            break;
        case 'd':
        case 'i':
            params->type = TYPE_SIGNED_INT;
            break;
        case 'u':
            params->type = TYPE_UNSIGNED_INT;
            break;
        case 'x':
        case 'X':
            params->type = TYPE_UNSIGNED_HEX;
            break;
        case 'o':
            params->type = TYPE_UNSIGNED_OCT;
            break;
        case 's':
            params->type = TYPE_STRING;
            break;
        case 'c':
            params->type = TYPE_CHAR;
            break;
        case 'p':
            params->type = TYPE_POINTER;
            break;
        case 'n':
            params->type = TYPE_NOTHING;
            break;
        case 'a':
        case 'A':
        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
        default:
            fprintf(stderr, "<unsupported type '%c'>", **ptr);
            break;
    }
    params->state = STATE_PRINT;
    if (**ptr >= 'A' && **ptr <= 'Z') {
        params->extra |= EXTRA_TYPE_UPPER;
    }
    (*ptr)++;
}

void print_string(Params* params, va_list* vargs) {
    if (params->type == TYPE_STRING && params->extra & EXTRA_PRECISION_STAR) {
        params->precision = va_arg(*vargs, int);
    }

    char* str = va_arg(*vargs, char*);
    writes(params, str, vargs);
}

char digits[] = "0123456789abcdef";
void print_integer(Params* params, va_list* vargs, uint8_t base, bool is_signed) {
    long long value;
    switch (params->length) {
        case LENGTH_INT_CHAR:
            value = is_signed ? (long long) (char) va_arg(*vargs, int) : (long long) (unsigned char) va_arg(*vargs, unsigned int);
            break;
        case LENGTH_INT_SHORT:
            value = is_signed ? (long long) (short) va_arg(*vargs, int) : (long long) (unsigned short) va_arg(*vargs, unsigned int);
            break;
        case LENGTH_LONG:
            value = is_signed ? (long long) va_arg(*vargs, long) : (long long) va_arg(*vargs, unsigned long);
            break;
        case LENGTH_LONG_LONG:
            value = is_signed ? va_arg(*vargs, long long) : (long long) va_arg(*vargs, unsigned long long);
            break;
        case LENGTH_SIZE_T:
            value = (long long) va_arg(*vargs, size_t);
            is_signed= false;
            break;
        case LENGTH_INT_MAX_T:
            value = is_signed ? (long long) va_arg(*vargs, intmax_t) : (long long) va_arg(*vargs, uintmax_t);
            break;
        case LENGTH_PTR_DIFF_T:
            value = (long long) va_arg(*vargs, ptrdiff_t);
            is_signed = true;
            break;
        case LENGTH_LONG_DOUBLE:
            fprintf(stderr, "<unsupported length '%u'>", params->length);
            return;
        default:
            value = is_signed ? (long long) va_arg(*vargs, int) : (long long) va_arg(*vargs, unsigned int);
            break;
    }

    bool negative;
    unsigned long long absolute_value;
    if (is_signed && value < 0) {
        negative = true;
        absolute_value = (unsigned long long) (-value);
    } else {
        negative = false;
        absolute_value = (unsigned long long) value;
    }

    uint8_t i = 0;
    char buffer[25] = {0}; // in base 8, 3bits per digits = 22 char + one for the sign + two for '0x'
    do {
        buffer[i++] = digits[absolute_value % base];
        absolute_value /= base;
    } while (absolute_value != 0);

    if (negative == true) {
        buffer[i++] = '-';
    } else if (params->flags & FLAG_PLUS) {
        buffer[i++] = '+';
    } else if (params->flags & FLAG_SPACE) {
        buffer[i++] = ' ';
    }

    if (params->flags & FLAG_HASH &&
        (params->type == TYPE_UNSIGNED_HEX || params->type == TYPE_POINTER)) {
        buffer[i++] = 'x';
        buffer[i++] = '0';
    }

    if (params->extra & EXTRA_TYPE_UPPER) {
        for (uint8_t j = 0; j < sizeof(buffer); j++) {
            char c = buffer[j];
            if (c >= 'a' && c <= 'f') {
                buffer[j] = (char) (c - ' ');
            }
        }
    }
    strrev(buffer);
    writes(params, buffer, vargs);
}

void print_pointer(Params* params, va_list* vargs) {
#if __WORDSIZE == 64
    params->length = LENGTH_LONG_LONG;
#else
    params->length = LENGTH_LONG;
#endif
    params->flags |= FLAG_HASH;
    print_integer(params, vargs, 16, false);
}

void print_char(Params* params, va_list* vargs) {
    char c = va_arg(*vargs, int);
    fputc(c, params->stream);
}

void print_nothing(Params* params, va_list* vargs) {
    int* n_ptr = va_arg(*vargs, int*);
    *n_ptr = params->written;
}

void print(Params* params, va_list* vargs) {
    switch (params->type) {
        case TYPE_PERCENT:
            fputc('%', params->stream);
            break;
        case TYPE_CHAR:
            print_char(params, vargs);
            break;
        case TYPE_STRING:
            print_string(params, vargs);
            break;
        case TYPE_SIGNED_INT:
            print_integer(params, vargs, 10, true);
            break;
        case TYPE_UNSIGNED_INT:
            print_integer(params, vargs, 10, false);
            break;
        case TYPE_UNSIGNED_OCT:
            print_integer(params, vargs, 8, false);
            break;
        case TYPE_UNSIGNED_HEX:
            print_integer(params, vargs, 16, false);
            break;
        case TYPE_POINTER:
            print_pointer(params, vargs);
            break;
        case TYPE_NOTHING:
            print_nothing(params, vargs);
            break;
        case TYPE_DOUBLE_FIXED:
        case TYPE_DOUBLE_STD:
        case TYPE_DOUBLE_EXP:
        case TYPE_DOUBLE_HEX:
            fprintf(stderr,  "<unimplemented type '%u'>", params->type);
    }
    params->state = STATE_START;
}

typedef void (*state_parser)(Params*, char**);
const state_parser state_parsers[] = {
        parse_start,
        parse_flags,
        parse_width,
        parse_precision,
        parse_length,
        parse_type,
};

int vfprintf(FILE* stream, const char* format, va_list vargs) {
    if (format == NULL) {
        return EOF;
    }

    Params params = {0};
    params.stream = stream;
    params.state = STATE_START;

    char* ptr = (char*) format;
    while (*ptr != '\0') {
        if (params.state != STATE_PRINT) {
            state_parser parser = state_parsers[params.state];
            parser(&params, &ptr);
        } else {
            print(&params, &vargs);
        }
    }
}
