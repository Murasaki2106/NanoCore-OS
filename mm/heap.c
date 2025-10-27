#include "heap.h"

// Define a 1MB heap
#define HEAP_SIZE (1024 * 1024)
static uint8_t g_heap[HEAP_SIZE];
static uint32_t g_next_free = 0; // The "bump pointer"

void heap_init(void) {
    g_next_free = (uint32_t)&g_heap[0];
}

// Allocate memory
void* kmalloc(size_t size) {
    if (g_next_free + size > (uint32_t)&g_heap[0] + HEAP_SIZE) {
        // Out of memory!
        return 0; 
    }

    void* ptr = (void*)g_next_free;
    g_next_free += size;
    
    // Optional: Align to 4 bytes
    if (g_next_free % 4 != 0) {
        g_next_free = (g_next_free + 3) & ~0x3;
    }
    
    return ptr;
}

// In a simple bump allocator, free does nothing.
// To implement a real free, you need a "free list".
void kfree(void* ptr) {
    // Stub
    (void)ptr; // Silence unused variable warning
}