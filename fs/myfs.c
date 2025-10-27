#include "myfs.h"
#include "../mm/heap.h"      // Needs Part 2's kmalloc
#include "../drivers/tty.h"   // Needs Part 1's kprintf
#include "../include/common.h"  // Needs common functions

// Our root directory
static myfs_file_t g_root_dir[MAX_FILES];
static int g_file_count = 0;

// Helper to find a file
static myfs_file_t* myfs_find_file(const char* filename) {
    for (int i = 0; i < g_file_count; i++) {
        if (strcmp(g_root_dir[i].name, filename) == 0) {
            return &g_root_dir[i];
        }
    }
    return 0; // Not found
}

void myfs_init(void) {
    g_file_count = 0;
    
    // Create a welcome file!
    const char* welcome = "Welcome to our OS filesystem!";
    myfs_write("welcome.txt", (uint8_t*)welcome, strlen(welcome));
}

int myfs_read(const char* filename, uint8_t* buffer) {
    myfs_file_t* file = myfs_find_file(filename);
    if (!file) return -1; // Not found
    
    memcpy(buffer, file->data, file->size);
    buffer[file->size] = '\0'; // Null-terminate for easy printing
    return file->size;
}

int myfs_write(const char* filename, uint8_t* data, uint32_t len) {
    if (len > MAX_FILE_SIZE) return -1; // File too big

    myfs_file_t* file = myfs_find_file(filename);
    if (!file) {
        // Create new file
        if (g_file_count >= MAX_FILES) return -1; // Directory full
        file = &g_root_dir[g_file_count++];
        strcpy(file->name, filename);
        file->data = kmalloc(MAX_FILE_SIZE); // Allocate space
        if (!file->data) return -1; // Out of memory
    }
    
    // Write data
    memcpy(file->data, data, len);
    file->size = len;
    return len;
}

// Implementation for the 'ls' command
void myfs_ls(void) {
    if (g_file_count == 0) {
        tty_writestring("No files in filesystem.\n");
        return;
    }
        
    for (int i = 0; i < g_file_count; i++) {
        tty_writestring(g_root_dir[i].name);
        tty_writestring("\n");
    }
}