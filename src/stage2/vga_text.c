#include "vga_text.h"
#include "typedefs.h"

uint8_t* vga_text_memory = (uint8_t*) 0xB8000;
int current = 0;

void vga_text_putc(const char c) {
    vga_text_memory[current] = c;
    current = current + 2;
}

void vga_text_puts(const char* string) {
    while (*string) {
        vga_text_putc(*string);
        string++;
    }
}