#include <stdio.h>
#include <string.h>

#define MAX_FILES 64
#define FILENAME_LEN 32

typedef struct {
    int is_used;             // 0 = free, 1 = file exists
    char name[FILENAME_LEN]; // filename
    int size;                // file size in bytes
    int start_block;         // start location
    int block_count;         // number of blocks used
} FileEntry;

typedef struct {
    FileEntry entries[MAX_FILES];
} FileTable;

int main() {
    // 🔹 1. Open your existing disk image file
    FILE *disk = fopen("C:\\Users\\Rudra Patel\\os_proj\\my_disk.img", "r+b");
    if (!disk) {
        perror("Disk open failed");
        return 1;
    }

    // 🔹 2. Create and initialize an empty FileTable
    FileTable table;
    memset(&table, 0, sizeof(FileTable));

    for (int i = 0; i < MAX_FILES; i++) {
        table.entries[i].is_used = 0;
    }

    // 🔹 3. Write FileTable to the beginning of the disk (Block 0)
    fseek(disk, 0, SEEK_SET);
    fwrite(&table, sizeof(FileTable), 1, disk);

    printf("✅ FileTable initialized successfully and written to my_disk.img\n");

    fclose(disk);
    return 0;
}
