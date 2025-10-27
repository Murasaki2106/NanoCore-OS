#include "keyboard.h"
#include "../cpu/idt.h"
#include "../include/common.h"
#include "../kernel/shell.h" // Send keypresses to the shell

// US QWERTY keyboard layout
unsigned char kbd_us[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0,
    ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
};

// The C-level interrupt handler for the keyboard
static void keyboard_callback(registers_t* regs) {
    // Read the scancode from the keyboard's data port
    uint8_t scancode = inb(0x60);
    
    // Check if this is a key release (bit 7 is set)
    if (!(scancode & 0x80)) {
        // It's a key press. Translate it and send to shell.
        char c = kbd_us[scancode];
        if (c) {
            shell_handle_keypress(c);
        }
    }
}

void keyboard_init(void) {
    // Register our callback for IRQ 1 (keyboard)
    register_interrupt_handler(33, keyboard_callback); 
}