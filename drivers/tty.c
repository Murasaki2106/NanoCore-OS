#include "tty.h"
#include "../include/types.h"
#include "../include/common.h"

// VGA video memory
static volatile uint16_t *vga_buffer = (uint16_t *)0xB8000;
static int tty_x = 0;
static int tty_y = 0;
static uint8_t vga_color = 0x0F; // White on black

// Put a single character on the screen
void tty_putentryat(char c, uint8_t color, int x, int y) {
    const int index = y * 80 + x;
    vga_buffer[index] = (uint16_t)c | (uint16_t)color << 8;
}

// Update the hardware cursor
static void update_cursor(void) {
    uint16_t pos = tty_y * 80 + tty_x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// Scroll the screen up one line
static void scroll(void) {
    // Move all lines up by one
    for (int y = 0; y < 24; y++) {
        memcpy((void*)(vga_buffer + y * 80), (void*)(vga_buffer + (y + 1) * 80), 80 * 2);
    }
    // Clear the last line
    for (int x = 0; x < 80; x++) {
        tty_putentryat(' ', vga_color, x, 24);
    }
    tty_y = 24;
}

void tty_init(void) {
    tty_x = 0;
    tty_y = 0;
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            tty_putentryat(' ', vga_color, x, y);
        }
    }
    update_cursor();
}

void tty_putchar(char c) {
    if (c == '\n') {
        tty_x = 0;
        tty_y++;
    } else {
        tty_putentryat(c, vga_color, tty_x, tty_y);
        tty_x++;
    }

    if (tty_x >= 80) {
        tty_x = 0;
        tty_y++;
    }
    
    if (tty_y >= 25) {
        scroll();
    }
    update_cursor();
}

void tty_writestring(const char *data) {
    while (*data) {
        tty_putchar(*data++);
    }
}

void tty_backspace(void) {
    if (tty_x > 0) {
        tty_x--;
    } else if (tty_y > 0) {
        tty_x = 79;
        tty_y--;
    }
    
    tty_putentryat(' ', vga_color, tty_x, tty_y);
    update_cursor();
}