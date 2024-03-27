#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

void kprintf(const char* format, ...);

/**
 * A basic implementation of the standard printf.
 *
 * Format: %[flags][.precision][length]type
 *
 * Supported Flags :
 * - PLUS, SPACE, HASH
 *
 * Supported Precision:
 * - only with string
 *
 * Supported Length:
 * - hh, h, l, ll
 *
 * Supported Type:
 * - %, d, i, u, x, X, o, s, c, p
 */
// Using a state machine
#define STATE_START          0
#define STATE_FLAGS          1
#define STATE_WIDTH          2  // unsupported
#define STATE_PRECISION      3
#define STATE_LENGTH         4
#define STATE_TYPE           5
#define STATE_PRINT          6

// FLAGS = 0 = undefined
#define FLAG_MINUS           0b000001    // unsupported
#define FLAG_PLUS            0b000010
#define FLAG_SPACE           0b000100
#define FLAG_ZERO            0b001000    // unsupported
#define FLAG_APOSTROPHE      0b010000    // unsupported
#define FLAG_HASH            0b100000    // unsupported

// LENGTH = 0 = undefined
#define LENGTH_INT_CHAR      1
#define LENGTH_INT_SHORT     2
#define LENGTH_LONG          3
#define LENGTH_LONG_LONG     4
#define LENGTH_LONG_DOUBLE   5   // unsupported
#define LENGTH_SIZE_T        6
#define LENGTH_INT_MAX_T     7   // unsupported
#define LENGTH_PTR_DIFF_T    8   // unsupported

// Types
#define TYPE_PER_CENT        0
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
#define TYPE_NOTHING         12  // unsupported

#define EXTRA_TYPE_UPPER     0b001
#define EXTRA_WIDTH_STAR     0b010   // unsupported
#define EXTRA_PRECISION_STAR 0b100

typedef struct {
    uint8_t state;
    uint8_t flags;
    uint8_t width;
    int precision;
    uint8_t length;
    uint8_t type;
    uint8_t extra;
    int written;
} Params;

void reset(Params * params) {
    params->flags = 0;
    params->width = 0;
    params->precision = 0;
    params->length = 0;
    params->type = 0;
    params->extra = 0;
}

void writec(Params* params, char c) {
    putchar(c);
    params->written++;
}

void handle_start(Params* params, char** ptr) {
    reset(params);
    if (**ptr == '%') {
        params->state = STATE_FLAGS;
    } else {
        writec(params, **ptr);
    }
    (*ptr)++;
}

void handle_flags(Params* params, char** ptr) {
    switch (**ptr) {
        case '-':
        case '\'':
        case '#':
            kprintf("<unsupported flags '%c'>", **ptr);
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

void handle_width(Params* params, char** ptr) {
    if (**ptr >= '0' && **ptr <= '9') {
        params->width = params->width * 10 + (**ptr - '0');
        (*ptr)++;
    } else if (**ptr == '*') {
        if (params->width == 0) {
            params->extra |= EXTRA_WIDTH_STAR;
        } else {
            kprintf("<invalid width '%u*'>", params->width);
        }
        (*ptr)++;
    } else if (**ptr == '.') {
        params->state = STATE_PRECISION;
        (*ptr)++;
    } else {
        params->state = STATE_LENGTH;
    }
}

void handle_precision(Params* params, char** ptr) {
    if (**ptr >= '0' && **ptr <= '9') {
        params->precision = params->precision * 10 + (**ptr - '0');
        (*ptr)++;
    } else if (**ptr == '*') {
        if (params->precision == 0) {
            params->extra |= EXTRA_PRECISION_STAR;
        } else {
            kprintf("<invalid precision '%u*'", params->precision);
        }
        (*ptr)++;
    } else {
        params->state = STATE_LENGTH;
    }
}

void handle_length(Params* params, char** ptr) {
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

void handle_type(Params* params, char** ptr) {
    switch (**ptr) {
        case '%':
            params->type = TYPE_PER_CENT;
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
        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
        default:
            kprintf("<unsupported type '%c'>", **ptr);
            break;
    }
    params->state = STATE_PRINT;
    if (**ptr >= 'A' && **ptr <= 'Z') {
        params->extra |= EXTRA_TYPE_UPPER;
    }
    (*ptr)++;
}

void print_string(Params* params, va_list* vargs) {
    int precision = params->precision;
    if (params->extra & EXTRA_PRECISION_STAR) {
        precision = va_arg(*vargs, int);
    }

    char* str = va_arg(*vargs, char*);
    for (size_t i = 0; *str != '\0'; i++, str++) {
        writec(params, *str);
        if (precision != 0 && i > precision) {
            break;
        }
    }
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
            break;
        case LENGTH_INT_MAX_T:
        case LENGTH_PTR_DIFF_T:
        case LENGTH_LONG_DOUBLE:
            kprintf("<unsupported length '%u'>", params->length);
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
    char buffer[20];
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

    while (i > 0) {
        char c = buffer[--i];
        if (c >= 'a' && c <= 'f' && params->extra & EXTRA_TYPE_UPPER) {
            c -= ' '; // convert to uppercase
        }
        writec(params, c);
    }
}

void print_pointer(Params* params, va_list* vargs) {
    uintptr_t value = va_arg(*vargs, uintptr_t);
    uint8_t ptr_size = sizeof(uintptr_t);
    uint8_t nb_digits = ptr_size * 2;

    writec(params, '0');
    writec(params, 'x');
    for (int i = nb_digits - 1; i >= 0; i--) {
        uint8_t hex_value = (value >> (i * 4)) & 0xF;
        writec(params, digits[hex_value]);
    }
}

void print_char(Params* params, va_list* vargs) {
    char c = va_arg(*vargs, int);
    writec(params, c);
}

void handle_print(Params* params, va_list* vargs) {
    switch (params->type) {
        case TYPE_PER_CENT:
            writec(params, '%');
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
        case TYPE_DOUBLE_FIXED:
        case TYPE_DOUBLE_STD:
        case TYPE_DOUBLE_EXP:
        case TYPE_DOUBLE_HEX:
            kprintf("<unimplemented type '%u'>", params->type);
    }
    params->state = STATE_START;
}

typedef void (*StateHandler_t)(Params*, char**);
const StateHandler_t state_handlers[] = {
        handle_start,
        handle_flags,
        handle_width,
        handle_precision,
        handle_length,
        handle_type,
};

void kprintf(const char* format, ...) {
    if (format == NULL) {
        return;
    }

    Params params = {0};
    params.state = STATE_START;

    va_list vargs;
    va_start(vargs, format);

    char* ptr = (char*) format;
    while (*ptr != '\0') {
        if (params.state != STATE_PRINT) {
            StateHandler_t state_handler = state_handlers[params.state];
            state_handler(&params, &ptr);
        } else {
            handle_print(&params, &vargs);
        }
    }

    va_end(vargs);
}


int main(void) {
    printf("sizeof(char): %lu\n", sizeof(char));
    printf("sizeof(int): %lu\n", sizeof(int));
    printf("sizeof(uint8_t): %lu\n", sizeof(uint8_t));
    printf("sizeof(uint16_t): %lu\n", sizeof(uint16_t));
    printf("sizeof(uint32_t): %lu\n", sizeof(uint32_t));
    printf("sizeof(uint64_t): %lu\n", sizeof(uint64_t));
    printf("sizeof(size_t): %lu\n", sizeof(size_t));
    printf("sizeof(uintptr_t): %lu\n", sizeof(uintptr_t));

    printf("Hello\n");
    kprintf("Hello\n");
    printf("---\n");

    // Caractères simples
    printf("%%c: %c\n", 'A');
    kprintf("%%c: %c\n", 'A');
    printf("---\n");

    // Chaînes de caractères
    printf("%%s: %s\n", "Hello, world!");
    kprintf("%%s: %s\n", "Hello, world!");
    printf("---\n");

    // Entiers signés
    printf("%%d: %d\n", 12345);
    kprintf("%%d: %d\n", 12345);
    printf("---\n");
    printf("%%d: %+d\n", 12);
    kprintf("%%d: %+d\n", 12);
    printf("---\n");
    printf("%%d: %+d\n", -12);
    kprintf("%%d: %+d\n", -12);
    printf("---\n");
    printf("%%d: % d\n", 12);
    kprintf("%%d: % d\n", 12);
    printf("---\n");
    printf("%%d: %4d\n", 1);
    kprintf("%%d: %4d\n", 1);
    printf("---\n");

    // Entiers non signés
    printf("%%u: %u\n", 54321);
    kprintf("%%u: %u\n", 54321);
    printf("---\n");

    // Octal
    printf("%%o: %o\n", 123);
    kprintf("%%o: %o\n", 123);
    printf("---\n");

    // Hexadécimal
    printf("%%x: %x\n", 0xABCD);
    kprintf("%%x: %x\n", 0xABCD);
    printf("---\n");
    printf("%%x: %X\n", 0xABCD);
    kprintf("%%x: %X\n", 0xABCD);
    printf("---\n");

    // Pointeurs
    int var = 42;
    printf("%%p: %p\n", (void*)&var);
    kprintf("%%p: %p\n", (void*)&var);
    printf("---\n");

    // Caractères spéciaux
    int a = 0;
    int b = 0;
    printf("%%n: %n\n", &a);
    kprintf("%%n: %n\n", &b);
    printf("res: %d\n", a);
    kprintf("res: %d\n", b);
    printf("---\n");

    // Flottants
    printf("%%f: %f\n", 3.14159);
    kprintf("%%f: %f\n", 3.14159);
    printf("---\n");

    // Largeur minimale
    printf("%5d\n", 10);
    kprintf("%5d\n", 10);
    printf("---\n");

    // Précision des décimales
    printf("%.2f\n", 3.14159);
    kprintf("%.2f\n", 3.14159);
    printf("---\n");

    // Combiner largeur minimale et précision
    printf("%10.4f\n", 123.456789);
    kprintf("%10.4f\n", 123.456789);
    printf("---\n");

    printf("%u\n", 0x0005);
    kprintf("%u\n", 0x0005);
    printf("---\n");

    uint16_t d = 8006;
    uint16_t c = *(uint16_t*) &d;
    printf("value: %u at %p\n", c, &d);
    kprintf("value: %u at %p\n", c, &d);
    printf("---\n");

    size_t size = 1234567890;
    printf("%%zd: %zd\n", size);
    kprintf("%%zd: %zd\n", size);

    size_t sizea = -1234567890;
    printf("%%zd: %zd\n", sizea);
    kprintf("%%zd: %zd\n", sizea);
}
