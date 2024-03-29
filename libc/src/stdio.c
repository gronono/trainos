#include <stdio.h>
#include <stddef.h>

int fputc(int ch, FILE* stream) {
    if (stream == NULL) {
        return EOF;
    }
    return stream->putc((unsigned char) ch);
}

int putc(int ch, FILE* stream) {
    return fputc(ch, stream);
}

int ferror(FILE* stream) {
    return stream->error;
}

int fprintf(FILE* stream, const char* format, ...) {
    va_list vargs;
    va_start(vargs, format);
    int result = vfprintf(stream, format, vargs);
    va_end(vargs);
    return result;
}

int printf(const char* format, ...) {
    va_list vargs;
    va_start(vargs, format);
    int result = vfprintf(stdout, format, vargs);
    va_end(vargs);
    return result;
}

int vprintf(const char* format, va_list vargs) {
    return vfprintf(stdout, format, vargs);
}
