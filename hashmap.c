#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

#define TABLE_SIZE 10007  // Prime number for better hash distribution

typedef struct HashNode {
    char word[MAX_WORD_LENGTH];
    struct HashNode *next;
} HashNode;

typedef struct HashMap {
    HashNode **table;
    int size;
} HashMap;

// Hash function
unsigned int hash(const char *word) {
    unsigned int hash_value = 0;
    while (*word) {
        hash_value = (hash_value * 31 + *word) % TABLE_SIZE;
        word++;
    }
    return hash_value;
}

// Create new hash map
HashMap* create_hashmap() {
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    map->size = TABLE_SIZE;
    map->table = (HashNode **)calloc(TABLE_SIZE, sizeof(HashNode *));
    return map;
}

// Insert into hash map
void insert_hashmap(HashMap *map, const char *word) {
    unsigned int index = hash(word);
    
    // Check if word already exists
    HashNode *current = map->table[index];
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            return;  // Word already exists
        }
        current = current->next;
    }

    // Insert new word at the beginning of the chain
    HashNode *node = (HashNode *)malloc(sizeof(HashNode));
    strcpy(node->word, word);
    node->next = map->table[index];
    map->table[index] = node;
}

// Search in hash map
int search_hashmap(HashMap *map, const char *word) {
    unsigned int index = hash(word);
    
    HashNode *current = map->table[index];
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            return 1;
        }
        current = current->next;
    }
    
    return 0;
}

// Load dictionary into hash map
HashMap* load_hashmap() {
    HashMap *map = create_hashmap();
    
    char **words;
    int count;
    load_dictionary(&words, &count);

    for (int i = 0; i < count; i++) {
        insert_hashmap(map, words[i]);
    }

    free_dictionary(words, count);
    return map;
}

// Free hash map
void free_hashmap(HashMap *map) {
    for (int i = 0; i < map->size; i++) {
        HashNode *current = map->table[i];
        while (current != NULL) {
            HashNode *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(map->table);
    free(map);
}

// Main function
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./hashmap [check|add|list] <word>\n");
        return 1;
    }

    char *operation = argv[1];
    to_lowercase(operation);

    if (strcmp(operation, "check") == 0) {
        if (argc < 3) {
            printf("Error: Word required for check operation\n");
            return 1;
        }

        char word[MAX_WORD_LENGTH];
        strcpy(word, argv[2]);
        to_lowercase(word);

        HashMap *map = load_hashmap();

        if (search_hashmap(map, word)) {
            printf("FOUND: '%s'\n", word);
        } else {
            printf("NOT_FOUND: '%s'\n", word);
        }

        free_hashmap(map);

    } else if (strcmp(operation, "add") == 0) {
        if (argc < 3) {
            printf("Error: Word required for add operation\n");
            return 1;
        }

        char word[MAX_WORD_LENGTH];
        strcpy(word, argv[2]);
        to_lowercase(word);

        HashMap *map = load_hashmap();

        if (search_hashmap(map, word)) {
            printf("EXISTS: '%s'\n", word);
            free_hashmap(map);
            return 0;
        }

        // Load all words, add new word, and save sorted
        char **words;
        int count;
        load_dictionary(&words, &count);

        // Add new word to array
        words = (char **)realloc(words, (count + 1) * sizeof(char *));
        words[count] = (char *)malloc(strlen(word) + 1);
        strcpy(words[count], word);
        count++;

        // Save sorted
        save_dictionary_sorted(words, count);

        printf("ADDED: '%s'\n", word);

        free_dictionary(words, count);
        free_hashmap(map);

    } else if (strcmp(operation, "list") == 0) {
        char **words;
        int count;
        load_dictionary(&words, &count);

        for (int i = 0; i < count; i++) {
            printf("%s\n", words[i]);
        }

        free_dictionary(words, count);

    } else {
        printf("Error: Unknown operation\n");
        return 1;
    }

    return 0;
}
