CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2
TARGET = spellchecker

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c

clean:
	rm -f $(TARGET)
