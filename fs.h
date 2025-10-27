#define MAX_FILENAME_LEN 32
#define MAX_FILES 64
#define MAX_BLOCKS_PER_FILE 8

// Your "File Table Entry"
// This is what the prompt calls "filename, size, block pointers"
typedef struct {
    char name[MAX_FILENAME_LEN];
    unsigned int size_in_bytes;
    int is_used; // 1 if this entry is active, 0 if free
    
    // Pointers to the data blocks that hold this file's content
    int data_block_indices[MAX_BLOCKS_PER_FILE]; 
} FileEntry;

// Your "File Table" is just an array of these
// We'll put this at Block 1 on the disk
typedef struct {
    FileEntry files[MAX_FILES];
} FileTable;

// (Optional but good practice)
// This goes at Block 0 on the disk
typedef struct {
    int total_blocks;
    int total_data_blocks;
    // ... any other metadata
} Superblock;