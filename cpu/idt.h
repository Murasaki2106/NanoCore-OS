#ifndef IDT_H
#define IDT_H

#include "../include/types.h"

// An entry in the Interrupt Descriptor Table
struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;     // Kernel segment selector
    uint8_t  always0; // This must always be zero
    uint8_t  flags;   // Flags
    uint16_t base_hi;
} __attribute__((packed));

// A pointer to the IDT
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// A struct to hold register state (pushed by isr.S)
typedef struct {
    uint32_t ds;                                     // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  // Pushed by 'pusha'
    uint32_t int_no, err_code;                       // Interrupt number and error code
    uint32_t eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically
} registers_t;

// A handler function is just a pointer to a function that takes registers_t*
typedef void (*isr_t)(registers_t*);

// Public functions
void idt_init(void);
void register_interrupt_handler(uint8_t n, isr_t handler);

#endif