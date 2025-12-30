# c-lineditor
A simple terminal-based line editor

### Description
The **c-lineditor** is a simple **terminal-based** [line editor](https://en.wikipedia.org/wiki/Line_editor) written in the C programming language that allows you to create and edit text files or even `code` through line-based operations including append, delete, edit, insert, quit, and save.

*This line editor was developed both as a learning exercise in dynamic memory management, pointer-based data structures, and modular program design in C, and as the final project for [Harvard's CS50x](https://cs50.harvard.edu/x/) course.*

### How it works
The editor requires the user to specify either an existing file or a new file via the first command-line argument `argv[1]`:
```
Usage: ./c-lineditor <file>
```
An empty `Document` structure is then created to represent the file. If the file exists, memory is allocated dynamically using `malloc()`, and the file's contents are then loaded into a resizable array of strings `char **lines` within the previously empty `Document` structure where each string now represents a single line of the file, with a maximum length of `MAX_LINE` (1024) characters. The `Document` structure also stores additional metadata such as the filename, the number of lines, whether the file is new, and whether it has been modified:

```
typedef struct Document {
    char **lines;
    int count;
    int changed;
    char *filename;
    int new;
} Document;
```

Once the file has been loaded, the editor operates entirely within the **terminal**, formatting and displaying the file’s name with its contents and allowing user interaction via a menu-based interface:
```
--- c-lineditor v0.1.0 ---

hello.c

  1 | #include <stdio.h>
  2 |
  3 | int main() {
  4 |     printf("Hello, World!\n");
  5 |     return 0;
  6 | }

Options:
[1] Edit
[2] Delete
[3] Append
[4] Insert
[5] Save and quit
[6] Quit
:
```
As the user changes the document by appending, deleting, or inserting lines, memory for the `char **lines` array is dynamically allocated and resized using `malloc()` and `realloc()` to reflect its state. Any unsaved changes are indicated to the user by a `*` suffix added to the filename. The user can then choose to save these changes or discard them before exiting the editor.

### Error Handling and Memory Safety

The editor carefully checks the return values of `malloc()`, `realloc()`, and `fopen()`. Temporary pointers are used when resizing memory to **avoid losing references** if allocation fails.

All allocated memory is explicitly **freed** before program exit to **prevent leaks**. Error cases are **handled** gracefully, with **informative messages** printed to the user when an operation fails.
