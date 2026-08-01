# c-lineditor
A lightweight terminal-based line editor.

### Description
The **c-lineditor** is a **terminal-based** [line editor](https://en.wikipedia.org/wiki/Line_editor) written in the C programming language that allows you to create and edit text files or even `code` through line-based operations including append, delete, edit, and insert.

*This line editor was developed both as a learning exercise in dynamic memory management, pointer-based data structures, and modular program design in C, and as the final project for [Harvard's CS50x](https://cs50.harvard.edu/x/) course.*

### How it works
The editor requires the user to specify either an existing file or a new file via the first command-line argument `argv[1]`:
```
Usage: ./c-lineditor <file>
```
An empty `Document` structure is then created to represent the file. If the file exists, memory is allocated dynamically using `malloc()`, and the file's contents are then loaded into a resizable array of strings `char **lines` within the previously empty `Document` structure where each string is dynamically sized to represent a single line of the file. The `Document` structure also stores additional metadata such as the filename, the number of lines, whether the file is new, and whether it has been modified:

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

hello.c*

  1 | #include <stdio.h>
  2 |
  3 | int main() {
  4 |     printf("Hello, World!\n");
  5 |     return 0;
  6 | }

Options:
[a]ppend
[d]elete
[e]dit
[i]nsert
[s]ave
[q]uit
:
```
As the user changes the document by appending, deleting, or inserting lines, the `char **lines` array is dynamically allocated and resized using `malloc()` and `realloc()` as needed. *Resizing is performed only when the array contains more than 50% unused capacity.* 

Any unsaved changes are indicated to the user by a `*` suffix added to the filename. The user can then choose to save these changes or discard them before exiting the editor.

### Error Handling and Memory Safety

The editor checks the return values of `malloc()`, `realloc()`, `fopen()`, and `strdup()`. Error cases are **handled** with **informative messages** through `perror()` or `fprintf()` directed to `stderr`. Temporary pointers are used when resizing memory to **avoid losing references** if allocation fails.

All allocated memory is explicitly **freed** before program exit to **prevent leaks**. 
