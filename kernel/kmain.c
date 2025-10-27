#include "../drivers/tty.h"
#include "../mm/heap.h"
#include "../fs/myfs.h"
#include "shell.h"

// This is the main C function called from boot.S
void kmain(void) {
    // 1. Initialize all basic drivers and systems
    tty_init();
    tty_writestring("Kernel booted successfully.\n");
    
    // TODO (Part 1): Initialize GDT, IDT, Keyboard, Timer
    // gdt_init();
    // idt_init();
    // keyboard_init();
    // timer_init(); // For Round Robin
    
    // 2. Initialize subsystems
    heap_init();     // (Part 2)
    myfs_init();     // (Part 3)
    
    tty_writestring("Heap and Filesystem initialized.\n");
    
    // 3. Start the shell (Final Stage)
    shell_init();
    
    // TODO (Part 1): After shell_init, you might want to
    // create_process(shell_run);
    // start_multitasking();
    
    // We should never get here
    while (1) {}
}