#ifndef EDITOR_H
#define EDITOR_H

// Maximum length per line
#define MAX_LINE 1024

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
int load_file(const char *filename, Document *my_file);
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
int get_option(char *c);
// Return an integer
int read_int(void);
// Return a pointer to a dynamically allocated string
char *read_line(FILE *fp);

// Operations

// Add a line at the end
int append_line(Document *my_file);
// Delete a specific line
int delete_line(Document *my_file);
// Edit a specific line
int edit_line(Document *my_file);
// Insert a line in a specific place
int insert_line(Document *my_file);
// Print the file content
void print_file(Document *my_file);

#endif
