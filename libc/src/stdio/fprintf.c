#include <stdio.h>

int fprintf(FILE* stream, const char* format, ...) {
    va_list vargs;
    va_start(vargs, format);
    int result = vfprintf(stream, format, vargs);
    va_end(vargs);
    return result;
}
