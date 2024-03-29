#include <stdio.h>
#include <stddef.h>

int fputc(int ch, FILE* stream) {
    if (stream == NULL) {
        return EOF;
    }
    return stream->putc((unsigned char) ch);
}
