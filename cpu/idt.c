#include "idt.h"
#include "isr.h"
#include "../include/common.h"
#include "../drivers/tty.h"

#define PIC1_CMD  0x20
#define PIC1_DATA 0x21
#define PIC2_CMD  0xA0
#define PIC2_DATA 0xA1

struct idt_entry idt[256];
struct idt_ptr idtp;

// Array of C-level interrupt handlers
isr_t interrupt_handlers[256];

// External assembly ISR stubs (from isr.S)
extern void isr0();
extern void isr1();
// ... (we only link a few for this example)
extern void isr33(); // Keyboard

// Set an IDT gate
static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel     = sel;
    idt[num].always0 = 0;
    idt[num].flags   = flags;
}

// Remap the PIC
static void pic_remap(void) {
    // Start initialization sequence
    outb(PIC1_CMD, 0x11);
    outb(PIC2_CMD, 0x11);
    
    // Set PIC vector offsets (ISR 32-47)
    outb(PIC1_DATA, 0x20); // PIC1 starts at 32
    outb(PIC2_DATA, 0x28); // PIC2 starts at 40
    
    // Tell PICs about each other
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    
    // Set 8086 mode
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    
    // Mask all interrupts (we'll unmask them later)
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

void idt_init(void) {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base  = (uint32_t)&idt;

    memset(&idt, 0, sizeof(struct idt_entry) * 256);
    memset(&interrupt_handlers, 0, sizeof(isr_t) * 256);

    // Remap the PIC
    pic_remap();

    // Set up IDT entries for ISRs
    // 0x08 is our kernel code segment
    // 0x8E is flags (present, ring 0, 32-bit interrupt gate)
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    // ... (set up more for CPU exceptions)
    
    // Set up the keyboard interrupt (IRQ 1, which is ISR 33)
    idt_set_gate(33, (uint32_t)isr33, 0x08, 0x8E);

    // Load the IDT
    idt_load((uint32_t)&idtp);

    // Unmask the keyboard interrupt line (0xFD = 11111101)
    outb(PIC1_DATA, 0xFD); 
    
    // Enable interrupts
    asm volatile ("sti");
}

// C-level handler called from assembly
void isr_handler(registers_t regs) {
    if (interrupt_handlers[regs.int_no] != 0) {
        interrupt_handlers[regs.int_no](&regs);
    } else {
        tty_writestring("Unhandled interrupt: ");
        // (Here you would write a function to print the interrupt number)
    }
}

// C-level handler for IRQs
void irq_handler(registers_t regs) {
    // Send End-of-Interrupt (EOI) signal to PICs
    if (regs.int_no >= 40) {
        outb(PIC2_CMD, 0x20); // EOI to slave
    }
    outb(PIC1_CMD, 0x20); // EOI to master

    // Call the registered C handler, if any
    if (interrupt_handlers[regs.int_no] != 0) {
        interrupt_handlers[regs.int_no](&regs);
    }
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}