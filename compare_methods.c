/*
 * Performance Comparison: BST vs HashMap
 * 
 * This program compares the performance of Binary Search Tree
 * and Hash Map implementations for dictionary operations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "hashmap.h"

#define MAX_WORD_LEN 256
#define DICT_FILE "dictionary.txt"

typedef struct BSTNode {
    char *word;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

BSTNode *bst_create_node(const char *word) {
    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));
    if (!node) return NULL;
    node->word = strdup(word);
    if (!node->word) {
        free(node);
        return NULL;
    }
    node->left = node->right = NULL;
    return node;
}

BSTNode *bst_insert(BSTNode *root, const char *word) {
    if (!root) return bst_create_node(word);
    
    int cmp = strcmp(word, root->word);
    if (cmp == 0) return root;
    if (cmp < 0) root->left = bst_insert(root->left, word);
    else root->right = bst_insert(root->right, word);
    
    return root;
}

int bst_search(BSTNode *root, const char *word) {
    if (!root) return 0;
    int cmp = strcmp(word, root->word);
    if (cmp == 0) return 1;
    if (cmp < 0) return bst_search(root->left, word);
    return bst_search(root->right, word);
}

void bst_free(BSTNode *root) {
    if (!root) return;
    bst_free(root->left);
    bst_free(root->right);
    free(root->word);
    free(root);
}

void to_lowercase(char *s) {
    for (; *s; ++s) *s = (char)tolower((unsigned char)*s);
}

double get_time_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
}

int main() {
    printf("\n");
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║  Performance Comparison: BST vs HashMap       ║\n");
    printf("╚════════════════════════════════════════════════╝\n\n");

    FILE *f = fopen(DICT_FILE, "r");
    if (!f) {
        printf("Error: Cannot open %s\n", DICT_FILE);
        return 1;
    }

    char **words = NULL;
    size_t word_count = 0;
    size_t word_capacity = 100;
    
    words = (char **)malloc(word_capacity * sizeof(char *));
    if (!words) {
        fclose(f);
        return 1;
    }

    char buffer[MAX_WORD_LEN];
    while (fgets(buffer, sizeof(buffer), f)) {
        char *p = buffer;
        while (*p && isspace((unsigned char)*p)) ++p;
        if (*p == '\0') continue;
        
        char *end = p + strlen(p) - 1;
        while (end > p && isspace((unsigned char)*end)) *end-- = '\0';
        
        to_lowercase(p);
        
        if (word_count >= word_capacity) {
            word_capacity *= 2;
            char **new_words = (char **)realloc(words, word_capacity * sizeof(char *));
            if (!new_words) {
                fclose(f);
                return 1;
            }
            words = new_words;
        }
        
        words[word_count++] = strdup(p);
    }
    fclose(f);

    printf("Loaded %zu words from %s\n\n", word_count, DICT_FILE);

    printf("Building data structures...\n");
    
    double bst_start = get_time_ms();
    BSTNode *bst_root = NULL;
    for (size_t i = 0; i < word_count; i++) {
        bst_root = bst_insert(bst_root, words[i]);
    }
    double bst_build_time = get_time_ms() - bst_start;
    
    double hashmap_start = get_time_ms();
    HashMap *hashmap = hashmap_create(0);
    for (size_t i = 0; i < word_count; i++) {
        hashmap_insert(hashmap, words[i]);
    }
    double hashmap_build_time = get_time_ms() - hashmap_start;

    printf("\n┌─────────────────────────────────────────────┐\n");
    printf("│  BUILD TIME COMPARISON                        │\n");
    printf("├───────────────────────────────────────────────┤\n");
    printf("│  BST:     %10.4f ms                           │\n", bst_build_time);
    printf("│  HashMap: %10.4f ms                           │\n", hashmap_build_time);
    printf("│  Winner:  %-30s                               │\n", 
           hashmap_build_time < bst_build_time ? "HashMap" : "BST");
    printf("└───────────────────────────────────────────────┘\n\n");

    printf("Running search benchmarks...\n\n");

    const char *test_words[] = {
        "apple", "zebra", "cat", "dog", "elephant",
        "notfound", "xyz", "test", "hello", "world"
    };
    size_t num_tests = sizeof(test_words) / sizeof(test_words[0]);

    double bst_total_search = 0;
    double hashmap_total_search = 0;

    for (size_t i = 0; i < num_tests; i++) {
        double start = get_time_ms();
        bst_search(bst_root, test_words[i]);
        double bst_time = get_time_ms() - start;
        bst_total_search += bst_time;

        start = get_time_ms();
        hashmap_search(hashmap, test_words[i]);
        double hashmap_time = get_time_ms() - start;
        hashmap_total_search += hashmap_time;
    }

    double bst_avg = bst_total_search / num_tests;
    double hashmap_avg = hashmap_total_search / num_tests;

    printf("┌─────────────────────────────────────────────┐\n");
    printf("│  SEARCH TIME COMPARISON (%zu searches)      │\n", num_tests);
    printf("├─────────────────────────────────────────────┤\n");
    printf("│  BST:     %10.6f ms (avg)                   │\n", bst_avg);
    printf("│  HashMap: %10.6f ms (avg)                   │\n", hashmap_avg);
    printf("│  Speedup: %.2fx                             │\n", bst_avg / hashmap_avg);
    printf("│  Winner:  %-30s  │\n",
           hashmap_avg < bst_avg ? "HashMap" : "BST");
    printf("└─────────────────────────────────────────────┘\n\n");

    printf("┌─────────────────────────────────────────────┐\n");
    printf("│  COMPLEXITY ANALYSIS                        │\n");
    printf("├─────────────────────────────────────────────┤\n");
    printf("│  Operation  │  BST (avg)  │  HashMap (avg) │\n");
    printf("├─────────────┼─────────────┼────────────────┤\n");
    printf("│  Search     │  O(log n)   │  O(1)          │\n");
    printf("│  Insert     │  O(log n)   │  O(1)          │\n");
    printf("│  Delete     │  O(log n)   │  O(1)          │\n");
    printf("│  Traverse   │  O(n)       │  O(n)          │\n");
    printf("└─────────────┴─────────────┴────────────────┘\n\n");

    printf("┌─────────────────────────────────────────────┐\n");
    printf("│  MEMORY ANALYSIS                            │\n");
    printf("├─────────────────────────────────────────────┤\n");
    printf("│  BST:     ~%.2f KB                         │\n", 
           (word_count * (sizeof(BSTNode) + 20)) / 1024.0);
    printf("│  HashMap: ~%.2f KB                          │\n",
           (hashmap->capacity * sizeof(HashNode*) + 
            word_count * (sizeof(HashNode) + 20)) / 1024.0);
    printf("└─────────────────────────────────────────────┘\n\n");

    printf("Conclusion:\n");
    printf("  - HashMap is faster for search operations (O(1) vs O(log n))\n");
    printf("  - HashMap uses more memory due to hash table overhead\n");
    printf("  - BST maintains sorted order naturally\n");
    printf("  - Choose HashMap for speed, BST for memory efficiency\n\n");

    bst_free(bst_root);
    hashmap_destroy(hashmap);
    
    for (size_t i = 0; i < word_count; i++) {
        free(words[i]);
    }
    free(words);

    return 0;
}
