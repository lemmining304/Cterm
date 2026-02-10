CC = gcc
CFLAGS = -Wall -O2 -Iinclude
SRC = src/main.c src/shell.c src/terminal.c src/history.c src/utils.c
OBJ = $(SRC:.c=.o)
TARGET = cterm

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
