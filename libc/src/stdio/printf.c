#include <stdio.h>

int printf(const char* format, ...) {
    va_list vargs;
    va_start(vargs, format);
    int result = vfprintf(stdout, format, vargs);
    va_end(vargs);
    return result;
}
