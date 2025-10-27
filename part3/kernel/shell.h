#ifndef SHELL_H
#define SHELL_H

void shell_init(void);
// This must be called by the keyboard driver (Part 1)
void shell_handle_keypress(char c);

#endif