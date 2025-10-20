CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2
TARGET = spellchecker
COMPARE = compare_methods
API = dictionary_api

all: $(TARGET) $(API)

$(TARGET): main.c hashmap.c hashmap.h
	$(CC) $(CFLAGS) -o $(TARGET) main.c hashmap.c

$(COMPARE): compare_methods.c hashmap.c hashmap.h
	$(CC) $(CFLAGS) -o $(COMPARE) compare_methods.c hashmap.c

$(API): dictionary_api.c hashmap.c hashmap.h
	$(CC) $(CFLAGS) -o $(API) dictionary_api.c hashmap.c

compare: $(COMPARE)

clean:
	rm -f $(TARGET) $(COMPARE) $(API)
