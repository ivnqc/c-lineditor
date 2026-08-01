CC = clang
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/main.c src/editor.c
TARGET = c-lineditor

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
