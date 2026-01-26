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
        fprintf(stderr, "Error loading file.\n");
        return 1;
    }

    // Track whether the user wants to save the file
    int save = 0;

    int running = 1;
    while (running)
    {
        print_file(&myfile);
        print_menu();

        char option;
        if (!get_option(&option))
        {
            printf("Invalid option.\n");
            continue;
        }

        switch (option)
        {
            case 'a': 
                append_line(&myfile); 
                break;
            case 'd': 
                delete_line(&myfile); 
                break;
            case 'e':
                edit_line(&myfile); 
                break;
            case 'i':
                insert_line(&myfile);
                break;
            case 's':
                save_file(&myfile);
                //save = 1;
                //running = 0;
                break;
            case 'q':
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
