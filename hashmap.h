#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

#define HASHMAP_INITIAL_SIZE 1024
#define HASHMAP_LOAD_FACTOR 0.75

typedef struct HashNode {
    char *key;
    struct HashNode *next;
} HashNode;

typedef struct HashMap {
    HashNode **buckets;
    size_t size;
    size_t capacity;
} HashMap;

HashMap *hashmap_create(size_t capacity);
void hashmap_destroy(HashMap *map);
int hashmap_insert(HashMap *map, const char *key);
int hashmap_search(HashMap *map, const char *key);
char **hashmap_get_all_words(HashMap *map, size_t *count);
void hashmap_free_words(char **words, size_t count);

#endif
