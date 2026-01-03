// Define EDITOR_H if it is not currently defined
#ifndef EDITOR_H
#define EDITOR_H

// Maximum length per line
#define MAX_LINE 1024

// Represent a text document/file, including its content and state
typedef struct Document {
    char **lines; // Allocated strings
    int count; // Total allocated strings
    int changed; // 0 when unchanged, 1 when changed
    char *filename;
    int new; // 0 when not new, 1 when new
} Document;

// Function declarations

// File I/O

// Load a file into memory
char **load_file(const char *filename, Document *my_file);
// Save the file and any changes made
int save_file(Document *my_file);

// Helpers

// Discard all leftover input from stdin until it reaches either '\n' or EOF
void clear_stdin(void);
// Initialize a Document structure
void document_init(Document *my_file);
// Deallocate requested memory
void free_file(Document *my_file);
// Read a single positive int from stdin, returns 1 on success, 0 on failure
int get_option(int *n);

// Operations

// Add a line at the end
char **append_line(Document *my_file);
// Delete a specific line
char **delete_line(Document *my_file);
// Edit a specific line
void edit_line(Document *my_file);
// Insert a line in a specific place
char **insert_line(Document *my_file);
// Print the file content
void print_file(Document *my_file)

#endif
