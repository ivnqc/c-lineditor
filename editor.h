#ifndef EDITOR_H
#define EDITOR_H

typedef struct Document {
    char **lines; // Allocated strings
    int count; // Total allocated strings
    int changed; // 0 when unchanged, 1 when changed
    char *filename;
    int new; // 0 when not new, 1 when new
} Document;

// Function declarations

// File I/O

// Load a file into a Document. Returns 1 on success, 0 on failure (memory allocation
// or I/O errors). Treats missing files (ENOENT) as success to allow creation of new files.
int load_file(const char *filename, Document *my_file);
// Write all lines from Document to file, one per line (newlines appended). Returns 1 on
// success, 0 on write failure. Clears the changed and new flags on success. File is overwritten.
int save_file(Document *my_file);

// Helpers

// Consume remaining input from stdin up to and including '\n', or until EOF.
// Useful after failed input validation to prevent leftover characters from affecting next read.
void clear_stdin(void);
// Zero-initialize a Document to empty state: no filename, no lines, not yet saved.
// Must be called before using Document. Does not allocate memory.
void document_init(Document *my_file);
// Free all allocated memory in Document (filename and all lines), then reinitialize.
// Safe to call on already-freed or uninitialized Documents.
void free_file(Document *my_file);
// Read a single alphabetic character from stdin and store its lowercase form in c.
// Returns 1 on success, 0 if input is not exactly one alphabetic character followed by newline.
int get_option(char *c);
// Print the main menu with available operations (append, delete, edit, insert, save, quit)
// and a prompt. Does not read input.
void print_menu(void);
// Read a line from stdin and parse it as a decimal integer. Returns the parsed integer
// on success, 0 on EOF, invalid input, or parse failure (non-numeric characters).
int read_int(void);
// Read a line from file up to '\n' or EOF into dynamically allocated memory.
// Returns a null-terminated string (newline stripped), or NULL on EOF with no input
// or on memory allocation failure. Caller must free the returned string.
char *read_line(FILE *fp);

// Operations

// Prompt user for a new line and append it to the Document. Returns 1 on success,
// 0 on EOF or memory allocation failure. Sets the changed flag on success.
int append_line(Document *my_file);
// Prompt user for a line number and delete that line from the Document. Returns 1 on success,
// 0 if no lines exist or invalid line number entered. Sets the changed flag on success.
int delete_line(Document *my_file);
// Prompt user for a line number and replacement text, then replace that line in the Document.
// Returns 1 on success, 0 if no lines exist, invalid line number, or EOF on input. Sets changed flag on success.
int edit_line(Document *my_file);
// Prompt user for a line number and new content, then insert the line before that position.
// If no lines exist, delegates to append_line(). Returns 1 on success, 0 on invalid input or EOF. Sets changed flag on success.
int insert_line(Document *my_file);
// Display the Document with header showing filename, changed status (*), and new file marker.
// Lines are numbered starting from 1. Shows empty marker if no lines exist. Does not modify Document.
void print_file(Document *my_file);

#endif
