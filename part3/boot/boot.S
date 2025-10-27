bits 32
 
; GRUB Multiboot header
section .multiboot
    align 4
    dd 0x1BADB002        ; Magic number
    dd 0x00              ; Flags
    dd - (0x1BADB002 + 0x00) ; Checksum
 
section .text
global start
extern kmain            ; Defined in kmain.c
 
start:
    ; Set up the stack pointer
    mov esp, kernel_stack_top
    
    ; Call our C kernel
    call kmain
    
    ; Halt the CPU if kmain ever returns
    cli
    hlt
 
section .bss
align 16
kernel_stack_bottom:
    resb 16384 ; 16KB stack
kernel_stack_top: