#include "shell.h"
#include "../drivers/tty.h"
#include "../drivers/keyboard.h" // (Must be created by Part 1)
#include "../include/common.h"
#include "../mm/heap.h"        // (From Part 2)
#include "../fs/myfs.h"        // (From Part 3)
#include "../proc/pcb.h"       // (From Part 1)
#include "../proc/scheduler.h" // (From Part 1)

#define MAX_CMD_LEN 100
static char g_cmd_buffer[MAX_CMD_LEN];
static int g_cmd_len = 0;

// (Helper functions for string splitting - you'll need to implement these)
// static int get_args(char* in, char** argv);
// static void run_command(char* cmd, int argc, char** argv);

void shell_init(void) {
    tty_writestring("\nMyOS Shell v1.0\n> ");
    g_cmd_len = 0;
    
    // IMPORTANT: Person 1 must implement the keyboard driver
    // and call shell_handle_keypress() on every key.
    // keyboard_init(shell_handle_keypress);
}

// This is the main command parser
void shell_run_command(char* cmd) {
    tty_putchar('\n');
    
    // TODO: Implement a real command parser that splits cmd into args
    // For now, we use simple strcmp
    
    if (strcmp(cmd, "ls") == 0) {
        // --- Part 3's Task ---
        myfs_ls();
    } 
    else if (strcmp(cmd, "ps") == 0) {
        // --- Part 2's Task (needs Part 1) ---
        tty_writestring("ps command not yet implemented.\n");
        // (Person 1/2 will loop through the PCB list)
        // scheduler_print_tasks(); 
    }
    else if (strcmp(cmd, "help") == 0) {
        tty_writestring("Commands: ls, ps, cat [file], echo [text] > [file], help\n");
    }
    else if (/* check for 'cat' */) {
        // --- Part 3's Task ---
        // char* filename = ... (get filename from cmd)
        // uint8_t* buffer = kmalloc(MAX_FILE_SIZE); (Uses Part 2)
        // int size = myfs_read(filename, buffer); (Uses Part 3)
        // if (size >= 0) {
        //     tty_writestring((char*)buffer);
        // } else {
        //     tty_writestring("File not found.\n");
        // }
        // kfree(buffer); (Uses Part 2)
        tty_writestring("cat command not yet implemented.\n");
    }
    else if (/* check for 'echo' */) {
        // --- Part 3's Task ---
        // (This is the most complex parsing)
        // char* text = ...
        // char* filename = ...
        // myfs_write(filename, (uint8_t*)text, strlen(text)); (Uses Part 3)
        tty_writestring("echo command not yet implemented.\n");
    }
    else if (/* check for 'kill' */) {
        // --- Part 2's Task ---
        // int pid = ... (get pid from cmd)
        // scheduler_kill_process(pid); (Uses Part 1)
        tty_writestring("kill command not yet implemented.\n");
    }
    else {
        tty_writestring("Unknown command: ");
        tty_writestring(cmd);
        tty_putchar('\n');
    }
    
    tty_writestring("> ");
    g_cmd_len = 0;
}

// This function is called by the keyboard driver
void shell_handle_keypress(char c) {
    if (c == '\n') {
        g_cmd_buffer[g_cmd_len] = '\0'; // Null-terminate the string
        shell_run_command(g_cmd_buffer);
    } 
    else if (c == '\b') { // Backspace
        if (g_cmd_len > 0) {
            g_cmd_len--;
            tty_putchar('\b'); // Handle backspace on screen
        }
    } 
    else if (g_cmd_len < MAX_CMD_LEN - 1) {
        g_cmd_buffer[g_cmd_len++] = c;
        tty_putchar(c);
    }
}