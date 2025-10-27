#ifndef COMMON_H
#define COMMON_H

#include "types.h"

// I/O Port Functions
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

// Memory Functions
void* memcpy(void* dest, const void* src, size_t count);
void* memset(void* dest, char val, size_t count);

// String Functions
int strcmp(const char* str1, const char* str2);
size_t strlen(const char* str);
char* strcpy(char* dest, const char* src);

#endif