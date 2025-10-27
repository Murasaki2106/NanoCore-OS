#include "gdt.h"
#include "../include/types.h"

// Our GDT with 3 entries: Null, Kernel Code, Kernel Data
struct gdt_entry gdt[3];
struct gdt_ptr gp;

// External assembly function to load the GDT
extern void gdt_flush(uint32_t);

// Set up a GDT entry
static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;
    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access      = access;
}

void gdt_init(void) {
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base  = (uint32_t)&gdt;

    // Null descriptor
    gdt_set_gate(0, 0, 0, 0, 0);
    // Code Segment (base=0, limit=4GB, 32-bit, 4KB granularity)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // Data Segment (same as code)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Load our new GDT
    gdt_flush((uint32_t)&gp);
}