#include <stdio.h>
#include <stdlib.h>

#include "editor.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    Document myfile;
    document_init(&myfile);

    if (!load_file(argv[1], &myfile))
    {
        printf("Error loading file.\n");
        return 1;
    }

    // Track whether the user wants to save the file
    int save = 0;

    int running = 1;
    while (running)
    {
        print_file(&myfile);

        printf("Options:\n");
        printf("[1] Edit\n");
        printf("[2] Delete\n");
        printf("[3] Append\n");
        printf("[4] Insert\n");
        printf("[5] Save and quit\n");
        printf("[6] Quit\n");
        printf(":");

        // Ensure the program proceeds only after a valid menu choice
        int option = 0;
        int valid = get_option(&option);

        if (!valid || (option < 1 || option > 6))
        {
            printf("Invalid option.\n");
            continue;
        }

        switch (option)
        {
            case 1: 
                edit_line(&myfile); 
                break;
            case 2: 
                delete_line(&myfile); 
                break;
            case 3:
                append_line(&myfile); 
                break;
            case 4:
                insert_line(&myfile);
                break;
            case 5:
                save = 1;
                running = 0;
                break;
            case 6:
                running = 0;
                break;
            default:
                continue;
        }
    }

    if (save)
    {
        if (save_file(&myfile))
            printf("File saved.\n");
        else
        {
            printf("Error saving file.\n");
            free_file(&myfile);
            return 1;
        }
    }
    else if (save == 0 && myfile.changed)
        printf("Changes discarded.\n");
    else
        printf("Bye!\n");

    free_file(&myfile);
    return 0;
}
