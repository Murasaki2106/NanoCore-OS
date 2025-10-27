#ifndef MYFS_H
#define MYFS_H

#include "../include/types.h"

#define MAX_FILENAME 16
#define MAX_FILES 16
#define MAX_FILE_SIZE 1024 // 1KB files

// A file "inode"
typedef struct {
    char name[MAX_FILENAME];
    uint32_t size;
    uint8_t* data; // Pointer to the file's data
} myfs_file_t;

// --- Public API ---
void myfs_init(void);
int myfs_read(const char* filename, uint8_t* buffer);
int myfs_write(const char* filename, uint8_t* data, uint32_t len);
void myfs_ls(void); // Function for the 'ls' command

#endif