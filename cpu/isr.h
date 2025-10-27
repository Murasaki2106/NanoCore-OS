#ifndef ISR_H
#define ISR_H

#include "idt.h"

// External assembly stubs
extern void isr0();
extern void isr1();
// ...
extern void isr31();

extern void isr32(); // IRQ 0 (Timer)
extern void isr33(); // IRQ 1 (Keyboard)
// ...
extern void isr47();

// Function to load the IDT (defined in isr.S)
extern void idt_load(uint32_t);

#endif