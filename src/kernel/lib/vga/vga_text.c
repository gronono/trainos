#include "vga_text.h"
#include "../port/port.h"
#include "../strings/strings.h"

#define VGA_CTRL_PORT   0x3D4
#define VGA_DATA_PORT   0x3D5
#define VGA_OFFSET_LOW  0x0F
#define VGA_OFFSET_HIGH 0x0E
#define VGA_ADDRESS     0xB8000

#define NB_ROWS 25
#define NB_COLS 80

uint16_t* vga_text_memory = (uint16_t*) VGA_ADDRESS;

struct Cursor {
    uint8_t row;
    uint8_t column;
    uint8_t foreground;
    uint8_t background;
} __attribute__((packed));

static struct Cursor* cursor;

uint16_t compute_index(const uint8_t row, const uint8_t column) {
    return row * NB_COLS + column;
}

void update_position() {
    uint16_t value = compute_index(cursor->row, cursor->column);
    port_write(VGA_CTRL_PORT, VGA_OFFSET_HIGH);
    port_write(VGA_DATA_PORT, value >> 8);

    port_write(VGA_CTRL_PORT, VGA_OFFSET_LOW);
    port_write(VGA_DATA_PORT, value & 0xFF);
}

void vga_text_set_colors(const enum VGAColor foreground, const enum VGAColor background) {
    cursor->foreground = foreground;
    cursor->background = background;
}

void vga_text_set_position(const uint8_t row, const uint8_t column) {
    cursor->column = column;
    cursor->row = row;
    update_position();
}

uint16_t compute_cursor_char(const uint8_t character) {
    uint8_t color = (cursor->background << 4) | cursor->foreground;
    return (color << 8) | character;
}

void vga_text_scroll() {
    // Move old buffer up
    for (uint8_t r = 1; r < NB_ROWS; r++ ) {
        for (uint8_t c = 0; c < NB_COLS; c++) {
            uint16_t oldIndex = compute_index(r, c);
            uint16_t newIndex = compute_index(r - 1, c);
            vga_text_memory[newIndex] = vga_text_memory[oldIndex];
        }
    }

    // Clear last row
    for (uint8_t c = 0; c < NB_COLS; c++) {
        uint16_t index = compute_index(NB_ROWS - 1, c);
        uint16_t coloredChar = compute_cursor_char(' ');
        vga_text_memory[index] = coloredChar;
    }
}

void vga_text_put_char(const char character) {
    if (character == '\n') {
        // new line
        cursor->row++;
        cursor->column = 0;
    } else if (character == '\b' && cursor->column > 0) {
        // backspace
        cursor->column--;
        vga_text_put_char(' ');
        cursor->column--;
    } else if (character >= ' ' && character <= '~') {
        uint16_t coloredChar = compute_cursor_char(character);
        uint16_t index = cursor->row * NB_COLS + cursor->column;
        vga_text_memory[index] = coloredChar;
        cursor->column++;
    } else {
        return;
    }

    if (cursor->column >= NB_COLS) {
        cursor->row++;
        cursor->column = 0;
    }
    if (cursor->row >= NB_ROWS) {
        vga_text_scroll();
        cursor->row = NB_ROWS - 1;
    }
    update_position();
}

void vga_text_put_string(const char* string) {
    print(vga_text_put_char, string);
}

void vga_clear_screen() {
    for (int i = 0; i < NB_COLS * NB_ROWS; i++) {
        vga_text_put_char(' ');
    }
    vga_text_set_position(0, 0);
}

void vga_text_init() {
    vga_text_set_colors(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK);
    vga_text_set_position(0, 0);
    vga_clear_screen();
}
