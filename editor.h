// Check if EDITOR_H is not currently defined, if not, define it
#ifndef EDITOR_H
#define EDITOR_H

// Maximum length per line
#define MAX_LINE 1024

// This structure represents a text document/file, including its content and state
typedef struct Document {
    char **lines; // Allocated strings
    int count; // Number of allocated strings
    int changed; // Flag changes
    char *filename;
    int new; // Flag new file
} Document;

// Function declarations

// Discard all leftover input from stdin until it reaches either '\n' or EOF
void clear_stdin(void);

// Try to read a single positive int from stdin and store it where '*n' points to
// returning 1 on success and 0 on failure
int get_option(int *n);

// Print the file content
void print_file(Document *my_file);

// Edit a specific line
void edit_line(Document *my_file);

// Delete a specific line
char **delete_line(Document *my_file);

// Add a line at the end
char **append_line(Document *my_file);

// Insert a line in a specific place
char **insert_line(Document *my_file);

// Load a file into memory
char **load_file(const char *filename, Document *my_file);

// Save the file and any changes made
int save_file(Document *my_file);

// Initialize a Document structure
void document_init(Document *my_file);

// Deallocate requested memory
void free_file(Document *my_file);

#endif
