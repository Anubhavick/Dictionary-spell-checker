CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGETS = bst hashmap

all: $(TARGETS)

bst: bst.c utils.c utils.h
	$(CC) $(CFLAGS) -o bst bst.c utils.c

hashmap: hashmap.c utils.c utils.h
	$(CC) $(CFLAGS) -o hashmap hashmap.c utils.c

clean:
	rm -f $(TARGETS)

.PHONY: all clean
