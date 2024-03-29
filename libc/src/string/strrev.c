#include "string.h"

void strrev(char* str) {
    size_t start = 0;
    size_t end = strlen(str) - 1;

    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

