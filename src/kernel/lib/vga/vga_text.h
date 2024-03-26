#pragma once

#include "../typedefs.h"

/**
 * Enumeration of standard VGA colors.
 */
enum VGAColor {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GRAY = 7,
    VGA_COLOR_DARK_GRAY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_YELLOW = 14,
    VGA_COLOR_WHITE = 15,
};

/**
 * Reset the VGA text display, setting up default colors, cursor position, and clearing the screen.
 */
void vga_text_reset();

/**
 * Clears the entire VGA text screen, filling it with spaces and resetting the cursor position to the top-left corner.
 */
void vga_clear_screen();

/**
 * This function sets the text display's foreground and background colors for all subsequent text output.
 * The color combination will be applied to all characters printed to the VGA text display until changed again.
 *
 * @param foreground The foreground color
 * @param background The background color
 */
void vga_text_set_colors(const enum VGAColor foreground, const enum VGAColor background);

/**
 * Sets the cursor position for the VGA text display.
 *
 * @param row The row where the cursor should be positioned (0-based).
 * @param column The column where the cursor should be positioned (0-based).
 */
void vga_text_set_position(const uint8_t row, const uint8_t column);

/**
 * Writes a character to the VGA text display at the current cursor position.
 *
 * @param character The character to be written.
 */
void vga_text_put_char(const char character);
