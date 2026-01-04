#include <stdio.h>
#include <stdlib.h>
#include "editor.h"


int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    // Initialize the structure for a potential new file
    Document myfile;
    document_init(&myfile);

    // Load the file supplied by the user
    load_file(argv[1], &myfile);
    // Ensure file loading
    if (!myfile.lines && !myfile.filename)
    {
        printf("Error loading file.\n");
        return 1;
    }

    // Flag
    int save = 0;

    // Run the menu
    int running = 1;
    while (running)
    {
        // Print the file content and its name
        print_file(&myfile);

        // Display the menu at the bottom
        printf("\nOptions:\n");
        printf("[1] Edit\n");
        printf("[2] Delete\n");
        printf("[3] Append\n");
        printf("[4] Insert\n");
        printf("[5] Save and quit\n");
        printf("[6] Quit\n");
        printf(":");

        // Prompt user
        int option = 0;
        int valid = get_option(&option);

        if (!valid || (option < 1 || option > 6))
        {
            printf("Invalid option.\n");
            continue;
        }

        // Execute user's option
        switch (option)
        {
            case 1: edit_line(&myfile); break;
            case 2: myfile.lines = delete_line(&myfile); break;
            case 3: myfile.lines = append_line(&myfile); break;
            case 4: myfile.lines = insert_line(&myfile); break;
            case 5: save = 1; running = 0; break;
            case 6: running = 0; break;
            default: continue; // "Handle" invalid option
        }
    }

    // Handle saving nuances
    if (save)
    {
        if (save_file(&myfile))
        {
            printf("File saved.\n");
        }
        else
        {
            printf("Error saving file.\n");
            // Deallocate memory
            free_file(&myfile);
            return 1;
        }
    }
    else if (save == 0 && myfile.changed)
    {
        printf("Changes discarded.\n");
    }
    else
    {
        printf("Bye!\n");
    }

    // Deallocate memory
    free_file(&myfile);

    return 0;
}
