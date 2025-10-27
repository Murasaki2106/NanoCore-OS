bits 32

; This macro creates a common ISR stub
%macro ISR_NOERR 1
global isr%1
isr%1:
    cli             ; Disable interrupts
    push byte 0     ; Push a dummy error code
    push byte %1    ; Push the interrupt number
    jmp isr_common_stub
%endmacro

; This macro creates an ISR stub for interrupts that push an error code
%macro ISR_ERR 1
global isr%1
isr%1:
    cli
    ; (error code is already on stack)
    push byte %1    ; Push the interrupt number
    jmp isr_common_stub
%endmacro

; This macro creates an IRQ stub
%macro IRQ 2
global isr%2
isr%2:
    cli
    push byte 0     ; Dummy error code
    push byte %2    ; Interrupt number
    jmp irq_common_stub
%endmacro

; CPU Exceptions
ISR_NOERR   0  ; Divide by zero
ISR_NOERR   1  ; Debug
ISR_NOERR   2  ; NMI
ISR_NOERR   3  ; Breakpoint
ISR_NOERR   4  ; Overflow
ISR_NOERR   5  ; Bound range exceeded
ISR_NOERR   6  ; Invalid opcode
ISR_NOERR   7  ; Device not available
ISR_ERR     8  ; Double fault
ISR_NOERR   9  ; Coprocessor segment overrun
ISR_ERR    10  ; Invalid TSS
ISR_ERR    11  ; Segment not present
ISR_ERR    12  ; Stack-segment fault
ISR_ERR    13  ; General protection fault
ISR_ERR    14  ; Page fault
ISR_NOERR  15  ; Reserved
ISR_NOERR  16  ; x87 FPU error
ISR_ERR    17  ; Alignment check
ISR_NOERR  18  ; Machine check
ISR_NOERR  19  ; SIMD FPU exception
; ... up to 31

; Hardware Interrupts (IRQs)
IRQ 0, 32    ; Timer (we aren't using it, but good to have)
IRQ 1, 33    ; Keyboard
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

extern isr_handler  ; This is our C-level handler

; This common stub saves the CPU state and calls the C handler
isr_common_stub:
    pusha           ; Push edi, esi, ebp, esp, ebx, edx, ecx, eax
    mov ax, ds      ; Get the data segment selector
    push eax        ; Push it onto the stack

    mov ax, 0x10    ; Load the kernel data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler ; Call our C handler
    
    pop eax         ; Reload original data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    popa            ; Pop eax, ecx, edx, ebx, esp, ebp, esi, edi
    add esp, 8      ; Clean up the error code and interrupt number
    iret            ; Pop eip, cs, eflags, useresp, ss

extern irq_handler ; C-level IRQ handler

; Common stub for IRQs (hardware interrupts)
irq_common_stub:
    pusha
    mov ax, ds
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call irq_handler ; Call our C IRQ handler
    
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    popa
    add esp, 8
    iret

; Function to load the IDT
global idt_load
idt_load:
    lidt [esp+4]
    ret