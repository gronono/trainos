#include "vga_text.h"
#include "typedefs.h"

const uint8_t VGA_WIDTH  = 80;
const uint8_t VGA_HEIGHT = 25;

struct Cursor {
    uint8_t row;
    uint8_t column;
    uint8_t foreground;
    uint8_t background;
};
struct Cursor cursor;

uint16_t* vga_text_memory = (uint16_t*) 0xB8000;

void vga_text_initialize() {
    cursor.row = 0;
    cursor.column = 0;
    cursor.foreground = VGA_COLOR_LIGHT_GRAY;
    cursor.background = VGA_COLOR_BLACK;
}

void vga_text_putc(const uint8_t character) {
    uint8_t  color = (cursor.background << 4) | cursor.foreground;
    uint16_t coloredChar = (color << 8) | character;
    uint16_t index = cursor.row * VGA_WIDTH + cursor.column;
    vga_text_memory[index] = coloredChar;
    cursor.column++;
    if (cursor.column >= VGA_WIDTH) {
        cursor.row++;
        cursor.column = 0;
    }
    if (cursor.row >= VGA_HEIGHT) {
        cursor.row = 0;
        vga_text_scroll();
    }
}

void vga_text_puts(const uint8_t* string) {
    while (*string != '\0') {
        vga_text_putc(*string);
        string++;
    }
}

void vga_text_set_cursor_colors(const enum VGAColor foreground, const enum VGAColor background) {
    cursor.foreground = foreground;
    cursor.background = background;
}

void vga_text_scroll() {
    // TODO
}
