#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "editor.h"

// Function definitions

// File I/O

// Load a file into memory
int load_file(const char *filename, Document *my_file)
{
    my_file->filename = strdup(filename);
    if (!my_file->filename)
    {
        fprintf(stderr, "%s: strdup failed.\n", __func__);
        return 0;
    }
    
    FILE *file = fopen(filename, "r");
    // Allow new files to be created while still reporting genuine I/O failures
    if (!file)
    {
        if (errno == ENOENT) return 1;
        
        perror("fopen failed");
        free_file(my_file);
        return 0;
    }

    char buffer[MAX_LINE];
    while (fgets(buffer, MAX_LINE, file))
    {
        char **new_size = realloc(my_file->lines, sizeof(char*) * (my_file->count + 1));
        if (!new_size)
        {
            fprintf(stderr, "%s: realloc failed.\n", __func__);
            free_file(my_file);
            return 0;
        }
        my_file->lines = new_size;

        my_file->lines[my_file->count] = malloc(strlen(buffer) + 1);
        if (!my_file->lines[my_file->count])
        {
            fprintf(stderr, "%s: malloc failed.\n", __func__);
            free_file(my_file);
            return 0;
        }
        strcpy(my_file->lines[my_file->count], buffer);
        my_file->count++;;
    }
    fclose(file);
    my_file->new = 0;
    return 1;
}

// Save the file and any changes made
int save_file(Document *my_file)
{
    FILE *file = fopen(my_file->filename, "w");
    if (!file) return 0;

    for (int i = 0; i < my_file->count; i++)
        fputs(my_file->lines[i], file);

    fclose(file);
    my_file->changed = 0;
    return 1;
}

// Helpers

// Discard all leftover input from stdin until it reaches either '\n' or EOF
void clear_stdin(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Initialize a Document structure
void document_init(Document *my_file)
{
    my_file->lines = NULL;
    my_file->count = 0;
    my_file->changed = 0;
    my_file->filename = NULL;
    my_file->new = 1;
}

// Deallocate requested memory
void free_file(Document *my_file)
{
    if (my_file->filename)
        free(my_file->filename);

    if (my_file->lines)
    {
        for (int i = 0; i < my_file->count; i++)
            free(my_file->lines[i]);
        
        free(my_file->lines);
    }
    document_init(my_file);
}

// Read a single positive int from stdin
int get_option(char *c)
{
    char buffer[3];

    if (!fgets(buffer, sizeof(buffer), stdin))
        return 0;

    if (buffer[0] == '\n' || buffer[1] != '\n')
        return 0;

    if (!isalpha((char)buffer[0]))
        return 0;

    *c = (char)tolower((char)buffer[0]);
    return 1;
}

// Operations

// Add a line at the end
int append_line(Document *my_file)
{
    char buffer[MAX_LINE];
    printf("Enter new line: ");
    fgets(buffer, MAX_LINE, stdin);

    char **new_size = realloc(my_file->lines, sizeof(char*) * (my_file->count + 1));
    if (!new_size)
    {
        fprintf(stderr, "%s: realloc failed.\n", __func__);
        return 0;
    }
    
    my_file->lines = new_size;

    my_file->lines[my_file->count] = malloc(strlen(buffer) + 1);
    if (!my_file->lines[my_file->count])
    {
        fprintf(stderr, "%s: malloc failed.\n", __func__);
        return 0;
    }

    strcpy(my_file->lines[my_file->count], buffer);

    my_file->count++;
    my_file->changed = 1;
    return 1;
}

// Delete a specific line
int delete_line(Document *my_file)
{
    if (!my_file->count)
    {
        printf("There are no text lines in the file.\n");
        return 0;
    }

    printf("Delete which line? (%d-%d): ", my_file->count ? 1 : 0, my_file->count);
    int line = 0;
    if (!get_option(&line) || line < 1 || line > my_file->count)
    {
        printf("Invalid line number\n");
        return 0;
    }

    free(my_file->lines[line - 1]);

    for (int i = line - 1; i < my_file->count - 1; i++)
        my_file->lines[i] = my_file->lines[i + 1];

    my_file->count--;

    if (my_file->count > 0)
    {
        char **new_size = realloc(my_file->lines, sizeof(char*) * my_file->count);
        if (new_size)
            my_file->lines = new_size;
        else
        {
            fprintf(stderr, "%s: realloc failed.\n", __func__);
            return 0;
        }
    }
    else
    {
        free(my_file->lines);
        my_file->lines = NULL;
        my_file->count = 0;
    }

    my_file->changed = 1;
    return 1;
}

// Edit a specific line
int edit_line(Document *my_file)
{
    if (!my_file->count)
    {
        printf("There are no text lines in the file.\nPlease use the 'Append' option.\n");
        return 0;
    }

    printf("Edit which line? (%d-%d): ", my_file->count ? 1 : 0, my_file->count);
    int line = 0;
    if (!get_option(&line) || line < 1 || line > my_file->count)
    {
        printf("Invalid line number\n");
        return 0;
    }

    printf("Current: %s", my_file->lines[line - 1]);
    char *current_line = my_file->lines[line - 1];

    printf("New line: ");
    char buffer[MAX_LINE];
    fgets(buffer, MAX_LINE, stdin);

    char *new_line = malloc(strlen(buffer) + 1);
    if (!new_line)
    {
        fprintf(stderr, "%s: malloc failed.\n", __func__);
        return 0;
    }

    strcpy(new_line, buffer);
    my_file->lines[line - 1] = new_line;

    free(current_line);
    
    my_file->changed = 1;
    return 1;
}

// Insert a line in a specific place
int insert_line(Document *my_file)
{
    if (!my_file->count)
        return append_line(my_file);
    
    printf("Insert at which line? (%d-%d): ", my_file->count ? 1 : 0, my_file->count ? my_file->count : 0);
    int line = 0;
    if (!get_option(&line) || line < 1 || line > my_file->count)
    {
        printf("Invalid line number\n");
        return 0;
    }

    printf("Enter new line: ");
    char buffer[MAX_LINE];
    fgets(buffer, MAX_LINE, stdin);

    char **new_size = realloc(my_file->lines, sizeof(char*) * (my_file->count + 1));
    if (!new_size)
    {
        fprintf(stderr, "%s: realloc failed.\n", __func__);
        return 0;
    }

    my_file->lines = new_size;

    char *new_line =  malloc(strlen(buffer) + 1);
    if (!new_line)
    {
        fprintf(stderr, "%s: malloc failed.\n", __func__);
        return 0;
    }

    for (int i = my_file->count; i > line - 1; i--)
        my_file->lines[i] = my_file->lines[i - 1];

    my_file->lines[line - 1] = new_line;
    strcpy(my_file->lines[line - 1], buffer);
    
    my_file->count++;
    my_file->changed = 1;
    return 1;
}

// Print the file content
void print_file(Document *my_file)
{
    printf("\n--- c-lineditor v0.1.0 ---\n");
    printf("\n%s%s %s\n", my_file->filename, my_file->changed ? "*" : "", my_file->new ? "(new file)" : "");
    printf("\n");

    if (!my_file->lines) printf("0 | \n");

    for (int i = 0; i < my_file->count; i++)
        printf("%3d | %s", i + 1, my_file->lines[i]);

    printf("\n");
}
