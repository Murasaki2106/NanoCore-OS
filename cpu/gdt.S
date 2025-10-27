bits 32
global gdt_flush

; Gets a pointer to the GDT structure in esp+4
gdt_flush:
    lgdt [esp+4]        ; Load the GDT pointer
    
    ; Reload segment registers to use the new GDT
    mov ax, 0x10        ; 0x10 is the offset for our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Far jump to reload the code segment register (cs)
    jmp 0x08:flush_cs   ; 0x08 is the offset for our code segment
    
flush_cs:
    ret