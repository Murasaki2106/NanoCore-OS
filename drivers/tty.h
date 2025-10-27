#ifndef TTY_H
#define TTY_H

void tty_init(void);
void tty_putchar(char c);
void tty_writestring(const char *data);
void tty_backspace(void); // For the shell

#endif