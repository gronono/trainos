#include "vga_text.h"

void start() {
    vga_text_initialize();

    vga_text_set_cursor_colors(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    int n = 25;
    for (int i = 0; i < n * 8; i++) {
        vga_text_puts("0123456789");
    }
    vga_text_set_cursor_colors(VGA_COLOR_RED, VGA_COLOR_BLACK);
    for (int i = 0; i < 25; i++)
        vga_text_puts(" Hello World! ");
    for (;;);
}