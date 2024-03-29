#include "stdio.h"

int vprintf(const char* format, va_list vargs) {
    return vfprintf(stdout, format, vargs);
}
