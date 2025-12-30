#include "editor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Function definitions

// Print the file content
void print_file(Document *my_file)
{
    // Print program's name and current version
    printf("\n--- c-lineditor v0.1.0 ---\n");

    // Print the name of the file appending an asterisk if file changed
    printf("\n%s%s %s\n", my_file->filename, my_file->changed ? "*" : "", my_file->new ? "(new file)" : "");

    printf("\n");

    if (my_file->lines)
    {
        // Pretty-print the allocated strings
        for (int i = 0; i < my_file->count; i++)
        {
            // i + 1 | my_file->lines[i]
            printf("%3d | %s", i + 1, my_file->lines[i]);
            //   1   | Hello, world!
        }
    }
    else
    {
        // Pretty-print an empty file
        printf("0 | \n");
    }

}

// Edit a specific line
void edit_line(Document *my_file)
{
    // Ensure there's something to edit
    if (!my_file->count)
    {
        printf("There are no text lines in the file.\nPlease use the 'Append' option.\n");
        return;
    }
    // Line to be edited
    int line = 0;

    // Temp storage for the new line
    char buffer[MAX_LINE];

    // Prompt user
    printf("Edit which line? (%d-%d): ", my_file->count ? 1 : 0, my_file->count);
    // Ensure line is a positive integer within files's bounds
    int valid = get_option(&line);
    if (!valid || (line < 1 || line > my_file->count))
    {
        printf("Invalid line number\n");
        return;
    }

    // Display the line to be edited
    printf("Current: %s", my_file->lines[line - 1]);
    // Save its memory address
    char *current_line = my_file->lines[line - 1];

    // Prompt user
    printf("New line: ");
    // Read from keyboard no more than MAX_LINE and put it in buffer
    fgets(buffer, MAX_LINE, stdin);

    // Allocate memory for the new line
    char *new_line = malloc(strlen(buffer) + 1);
    if (!new_line)
    {
        printf("malloc() failed.\n");
        return;
    }

    // Copy buffer into the successfully allocated memory
    strcpy(new_line, buffer);

    // Reassign pointer
    // Point to new_line instead of current_line
    my_file->lines[line - 1] = new_line;

    // Deallocate the old line
    free(current_line);

    // A change has been made to the file
    my_file->changed = 1;
}

// Delete a specific line
char **delete_line(Document *my_file)
{
    // Ensure there's something to delete
    if (!my_file->count)
    {
        printf("There are no text lines in the file.\n");
        // Return to the menu loop
        return my_file->lines;
    }

    // Line to be deleted
    int line = 0;

    // Prompt user
    printf("Delete which line? (%d-%d): ", my_file->count ? 1 : 0, my_file->count);
    // Ensure line is a positive integer within bounds
    int valid = get_option(&line);
    if (!valid || (line < 1 || line > my_file->count))
    {
        printf("Invalid line number\n");
        // Return to the menu loop
        return my_file->lines;
    }

    // Deallocate the selected line
    free(my_file->lines[line - 1]);

    // Shift lines "up" to fill the gap, which ends up with a duplicated
    // pointer at the end of the array
    for (int i = line - 1; i < my_file->count - 1; i++)
    {
        my_file->lines[i] = my_file->lines[i + 1];
    }

    // If there are still lines, shrink the array
    if ((my_file->count - 1) > 0)
    {
        // Request a smaller block of memory that is 8 * (my_file->count - 1) bytes
        // which deallocates the duplicated pointer at the end
        char **new_size = realloc(my_file->lines, sizeof(char*) * my_file->count - 1);
        if (new_size)
        {
            // Update pointer and count after successful reallocation
            my_file->lines = new_size;
            my_file->count--;
        }
        else
        {
            printf("realloc() failed.\n");
            // Return unshrinked array
            return my_file->lines;
        }
    }
    // There are no lines left
    else
    {
        // Deallocate memory
        free(my_file->lines);
        my_file->lines = NULL;
        my_file->count = 0;
    }

    // A change has been made to the file
    my_file->changed = 1;

    // Return the pointer
    return my_file->lines;
}

// Add a line at the end
char **append_line(Document *my_file)
{
    // Temp storage for the new line
    char buffer[MAX_LINE];

    // Prompt user
    printf("Enter new line: ");
    // Read from keyboard no more than MAX_LINE and put it in buffer
    fgets(buffer, MAX_LINE, stdin);

    // Request a bigger block of memory that is (8 * my_files->count + 1) bytes
    char **new_size = realloc(my_file->lines, sizeof(char*) * (my_file->count + 1));
    if (!new_size)
    {
        printf("realloc() failed.\n");
        // Return the same array
        return my_file->lines;
    }

    // Update pointer after successful reallocation
    my_file->lines = new_size;

    // Allocate memory for the new line
    my_file->lines[my_file->count] = malloc(strlen(buffer) + 1);
    if (!my_file->lines[my_file->count])
    {
        printf("malloc() failed.\n");
        // Return the array with an unused slot
        return my_file->lines;
    }

    // Copy buffer into the allocated memory
    strcpy(my_file->lines[my_file->count], buffer);

    // Update line count
    my_file->count++;

    // A change has been made to the file
    my_file->changed = 1;

    // Return updated pointer
    return my_file->lines;
}

// Insert a line in a specific place
char **insert_line(Document *my_file)
{
    // Call append_line if there are no lines
    if (!my_file->count)
    {
        return append_line(my_file);
    }

    // Line to be inserted
    int line = 0;
    // Temp storage for the new line
    char buffer[MAX_LINE];

    // Prompt user
    printf("Insert at which line? (%d-%d): ", my_file->count ? 1 : 0, my_file->count ? my_file->count : 0);
    int valid = get_option(&line);

    // Ensure line is a positive integer within bounds
    if (!valid || line < 1 || line > my_file->count)
    {
        printf("Invalid line number\n");
        // Return to the menu loop
        return my_file->lines;
    }

    // Prompt user
    printf("Enter new line: ");
    // Read from keyboard no more than MAX_LINE and put it in buffer
    fgets(buffer, MAX_LINE, stdin);

    // Request a bigger block of memory that is (8 * my_files->count + 1) bytes
    char **new_size = realloc(my_file->lines, sizeof(char*) * (my_file->count + 1));
    if (!new_size)
    {
        printf("realloc() failed.\n");
        // Return the same array
        return my_file->lines;
    }

    // Update pointer after successful reallocation
    my_file->lines = new_size;

    // Allocate memory for the new line
    char *new_line =  malloc(strlen(buffer) + 1);
    if (!new_line)
    {
        printf("malloc() failed.\n");
        // Return the array with an unused slot
        return my_file->lines;
    }


    // Shift lines "down" to make a gap
    // Starting from the end up to line
    for (int i = my_file->count; i > line - 1; i--)
    {
        my_file->lines[i] = my_file->lines[i - 1];
    }

    // Update pointer
    my_file->lines[line - 1] = new_line;

    // Copy buffer into the allocated memory
    strcpy(my_file->lines[line - 1], buffer);

    // Update line count
    my_file->count++;

    // A change has been made to the file
    my_file->changed = 1;

    // Return updated array
    return my_file->lines;

}

// Load a file into memory
char **load_file(const char *filename, Document *my_file)
{
    // Temp storage for each line
    char buffer[MAX_LINE];

    // Name the file
    my_file->filename = strdup(filename);
    if (!my_file->filename)
    {
        printf("strdup() failed.\n");
        return NULL;
    }
    // Open filename in read mode (*file points to it)
    FILE *file = fopen(filename, "r");
    // Return a pointer to a potential new file
    if (file == NULL && my_file->filename != NULL)
    {
        return my_file->lines;
    }

    // Loop read from file no more than MAX_LINE and put it into buffer
    while (fgets(buffer, MAX_LINE, file))
    {
        // Temporary pointer to prevent memory leak
        char **new_size = realloc(my_file->lines, sizeof(char*) * (my_file->count + 1));
        if (!new_size)
        {
            printf("realloc() failed.\n");
            free_file(my_file);
            return my_file->lines;
        }

        // Resize the lines pointer by (8 * count + 1) bytes
        my_file->lines = new_size;

        // Allocate memory for the new line
        my_file->lines[my_file->count] = malloc(strlen(buffer) + 1);
        if (!my_file->lines[my_file->count])
        {
            printf("malloc() failed.\n");
            free_file(my_file);
            return my_file->lines;
        }

        // Copy buffer into the allocated memory
        strcpy(my_file->lines[my_file->count], buffer);
        // Update line count
        my_file->count++;;
    }

    // Close the opened file
    fclose(file);

    // Not a new file since fgets succeed
    my_file->new = 0;

    // Return the lines pointer
    return my_file->lines;
}

// Save the file and any changes made
int save_file(Document *my_file)
{
    // Open filename in write mode (*file points to it)
    FILE *file = fopen(my_file->filename, "w");
    // Ensure file opened
    if (!file)
    {
        return 0;
    }

    // Loop through each line
    for (int i = 0; i < my_file->count; i++)
    {
        // Write lines[i] into file
        fputs(my_file->lines[i], file);
    }

    // Close the opened file
    fclose(file);

    // Save succeeded
    return 1;
}

// Initialize a Document structure
void document_init(Document *my_file)
{
    // Strings have not yet been allocated
    my_file->lines = NULL;
    my_file->count = 0;
    my_file->changed = 0;
    my_file->filename = NULL;
    my_file->new = 1;
}

// Deallocate requested memory
void free_file(Document *my_file)
{

    // Deallocate memory that could have been used by the new file
    if (my_file->lines == NULL && my_file->filename != NULL)
    {
        free(my_file->filename);
        my_file->filename = NULL;
        return;
    }

    // Deallocate strings
    for (int i = 0; i < my_file->count; i++)
    {
        free(my_file->lines[i]);
    }
    free(my_file->lines);
    free(my_file->filename);

    // Set the file to its initial state
    document_init(my_file);

}

// Discard all leftover input from stdin until it reaches either '\n' or EOF
void clear_stdin(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Try to read a single positive int from stdin and store it where '*n' points to
// returning 1 on success and 0 on failure
int get_option(int *n)
{
    char c;

    // Is it even a number?
    if (scanf("%d", n) != 1)
    {
        clear_stdin();
        return 0;
    }
    // Is it a positive number?
    if (*n <= 0)
    {
        clear_stdin();
        return 0;
    }
    // Is there any junk after the number?
    c = getchar();
    if (c != '\n')
    {
        clear_stdin();
        return 0;
    }

    return 1;
}
