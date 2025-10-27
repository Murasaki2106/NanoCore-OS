ENTRY(start) /* The entry point is 'start' in boot.S */
 
SECTIONS
{
    /* Start at 1MB. */
    . = 1M;
 
    .text :
    {
        *(.multiboot) /* Put the multiboot header first */
        *(.text)     /* All code */
    }
 
    .rodata :
    {
        *(.rodata)   /* All read-only data */
    }
 
    .data :
    {
        *(.data)     /* All initialized data */
    }
 
    .bss :
    {
        *(.bss)      /* All uninitialized data */
    }
}