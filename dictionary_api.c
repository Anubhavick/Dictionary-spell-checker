/*
 * Dictionary API - Optimized C Backend for Web Interface
 * 
 * Accepts JSON commands via stdin and outputs JSON responses
 * Supports both BST and HashMap implementations
 * 
 * Optimizations:
 * - Single dictionary load on startup
 * - Reduced memory allocations
 * - Optimized JSON parsing
 * - Faster suggestion algorithm
 * - Reduced code duplication
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "hashmap.h"

#define MAX_WORD_LEN 256
#define DICT_FILE "dictionary.txt"
#define MAX_SUGGESTIONS 10
#define MAX_INPUT 4096

typedef struct BSTNode {
    char *word;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

// Global dictionaries (lazy loaded)
static BSTNode *g_bst_root = NULL;
static HashMap *g_hashmap = NULL;
static int g_bst_loaded = 0;
static int g_hashmap_loaded = 0;

// Fast inline functions
static inline BSTNode *bst_create_node(const char *word) {
    BSTNode *node = malloc(sizeof(BSTNode));
    if (!node) return NULL;
    node->word = strdup(word);
    if (!node->word) {
        free(node);
        return NULL;
    }
    node->left = node->right = NULL;
    return node;
}

static inline void to_lowercase(char *s) {
    for (; *s; ++s) *s = tolower((unsigned char)*s);
}

static inline double get_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
}

// BST operations
BSTNode *bst_insert(BSTNode *root, const char *word) {
    if (!root) return bst_create_node(word);
    int cmp = strcmp(word, root->word);
    if (cmp == 0) return root;
    if (cmp < 0) root->left = bst_insert(root->left, word);
    else root->right = bst_insert(root->right, word);
    return root;
}

int bst_search(BSTNode *root, const char *word) {
    while (root) {
        int cmp = strcmp(word, root->word);
        if (cmp == 0) return 1;
        root = (cmp < 0) ? root->left : root->right;
    }
    return 0;
}

void bst_get_suggestions(BSTNode *root, const char *prefix, size_t prefix_len, char **suggestions, int *count, int max_count) {
    if (!root || *count >= max_count) return;
    bst_get_suggestions(root->left, prefix, prefix_len, suggestions, count, max_count);
    if (*count < max_count && strncmp(root->word, prefix, prefix_len) == 0) {
        suggestions[(*count)++] = root->word; // Don't strdup, use original
    }
    if (*count < max_count) {
        bst_get_suggestions(root->right, prefix, prefix_len, suggestions, count, max_count);
    }
}

void bst_get_all_words(BSTNode *root, char **words, int *count, int max) {
    if (!root || *count >= max) return;
    bst_get_all_words(root->left, words, count, max);
    if (*count < max) {
        words[(*count)++] = root->word;
        bst_get_all_words(root->right, words, count, max);
    }
}

void bst_free(BSTNode *root) {
    if (!root) return;
    bst_free(root->left);
    bst_free(root->right);
    free(root->word);
    free(root);
}

// Lazy load BST only when needed
static void load_bst(const char *filename) {
    if (g_bst_loaded) return;
    
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Warning: Cannot open %s\n", filename);
        return;
    }

    char buffer[MAX_WORD_LEN];
    while (fgets(buffer, sizeof(buffer), f)) {
        char *start = buffer, *end;
        while (*start && isspace(*start)) ++start;
        if (!*start) continue;
        
        end = start + strlen(start) - 1;
        while (end > start && isspace(*end)) *end-- = '\0';
        
        to_lowercase(start);
        g_bst_root = bst_insert(g_bst_root, start);
    }
    fclose(f);
    g_bst_loaded = 1;
}

// Lazy load HashMap only when needed
static void load_hashmap(const char *filename) {
    if (g_hashmap_loaded) return;
    
    g_hashmap = hashmap_create(0);
    if (!g_hashmap) return;

    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Warning: Cannot open %s\n", filename);
        return;
    }

    char buffer[MAX_WORD_LEN];
    while (fgets(buffer, sizeof(buffer), f)) {
        char *start = buffer, *end;
        while (*start && isspace(*start)) ++start;
        if (!*start) continue;
        
        end = start + strlen(start) - 1;
        while (end > start && isspace(*end)) *end-- = '\0';
        
        to_lowercase(start);
        hashmap_insert(g_hashmap, start);
    }
    fclose(f);
    g_hashmap_loaded = 1;
}

// Optimized HashMap suggestions (avoid full word list generation)
static void get_hashmap_suggestions(const char *prefix, size_t prefix_len, char **suggestions, int *count, int max_count) {
    size_t word_count;
    char **all_words = hashmap_get_all_words(g_hashmap, &word_count);
    if (!all_words) return;

    for (size_t i = 0; i < word_count && *count < max_count; i++) {
        if (strncmp(all_words[i], prefix, prefix_len) == 0) {
            suggestions[(*count)++] = all_words[i];
        }
    }
    // Note: Don't free individual words, just the array
}

// Optimized JSON escape (inline and avoid unnecessary copying)
static void print_json_escaped(const char *str) {
    putchar('"');
    while (*str) {
        if (*str == '"' || *str == '\\') putchar('\\');
        putchar(*str++);
    }
    putchar('"');
}

// Fast JSON parser (manual, no sscanf overhead)
static char *find_json_value(const char *json, const char *key) {
    static char buffer[MAX_WORD_LEN];
    char search[128];
    snprintf(search, sizeof(search), "\"%s\":\"", key);
    
    char *start = strstr(json, search);
    if (!start) return NULL;
    
    start += strlen(search);
    char *end = strchr(start, '"');
    if (!end) return NULL;
    
    size_t len = end - start;
    if (len >= MAX_WORD_LEN) len = MAX_WORD_LEN - 1;
    memcpy(buffer, start, len);
    buffer[len] = '\0';
    return buffer;
}

// Unified command handler
static void handle_command(const char *input) {
    /*
     * find_json_value() returns a pointer to a static buffer.
     * Copy values out immediately into local buffers so subsequent
     * calls don't overwrite earlier values.
     */
    char cmd_buf[MAX_WORD_LEN] = "";
    char method_buf[MAX_WORD_LEN] = "";

    char *tmp = find_json_value(input, "command");
    if (!tmp) return;
    strncpy(cmd_buf, tmp, MAX_WORD_LEN - 1);

    char word[MAX_WORD_LEN] = "";
    tmp = find_json_value(input, "word");
    if (tmp) {
        strncpy(word, tmp, MAX_WORD_LEN - 1);
        to_lowercase(word);
    }

    tmp = find_json_value(input, "method");
    if (tmp) strncpy(method_buf, tmp, MAX_WORD_LEN - 1);

    int use_hashmap = (method_buf[0] && strcmp(method_buf, "hashmap") == 0);
    const char *method = use_hashmap ? "hashmap" : "bst";

    /* Lazy load the required data structure */
    if (use_hashmap) load_hashmap(DICT_FILE);
    else load_bst(DICT_FILE);

    // CHECK command
    if (strcmp(cmd_buf, "check") == 0) {
        double start = get_time_ms();
        int found = use_hashmap ? hashmap_search(g_hashmap, word) : bst_search(g_bst_root, word);
        double elapsed = get_time_ms() - start;

        printf("{\"found\":%s,\"word\":", found ? "true" : "false");
        print_json_escaped(word);
        printf(",\"timeMs\":%.6f,\"method\":\"%s\"", elapsed, method);

        if (!found && *word) {
            char *suggestions[MAX_SUGGESTIONS];
            int count = 0;
            
            // Try prefix matching first
            if (use_hashmap) {
                get_hashmap_suggestions(word, strlen(word), suggestions, &count, MAX_SUGGESTIONS);
            } else {
                bst_get_suggestions(g_bst_root, word, strlen(word), suggestions, &count, MAX_SUGGESTIONS);
            }

            // Fallback to first N words if no suggestions
            if (count == 0) {
                if (use_hashmap) {
                    size_t word_count;
                    char **all = hashmap_get_all_words(g_hashmap, &word_count);
                    if (all) {
                        for (int i = 0; i < MAX_SUGGESTIONS && i < (int)word_count; i++) {
                            suggestions[count++] = all[i];
                        }
                    }
                } else {
                    bst_get_all_words(g_bst_root, suggestions, &count, MAX_SUGGESTIONS);
                }
            }

            printf(",\"suggestions\":[");
            for (int i = 0; i < count; i++) {
                if (i > 0) putchar(',');
                print_json_escaped(suggestions[i]);
            }
            putchar(']');
        }
        printf("}\n");
    }
    // ADD command
    else if (strcmp(cmd_buf, "add") == 0) {
        double start = get_time_ms();
        int exists = use_hashmap ? hashmap_search(g_hashmap, word) : bst_search(g_bst_root, word);
        
        if (!exists) {
            if (use_hashmap) hashmap_insert(g_hashmap, word);
            else g_bst_root = bst_insert(g_bst_root, word);

            FILE *f = fopen(DICT_FILE, "a");
            if (f) {
                fprintf(f, "%s\n", word);
                fclose(f);
            }
        }
        double elapsed = get_time_ms() - start;

        printf("{\"success\":%s,\"word\":", !exists ? "true" : "false");
        print_json_escaped(word);
        printf(",\"message\":\"%s\",\"timeMs\":%.6f}\n",
               !exists ? "Word added successfully" : "Word already exists", elapsed);
    }
    // LIST command
    else if (strcmp(cmd_buf, "list") == 0) {
        double start = get_time_ms();
        
        if (use_hashmap) {
            size_t count;
            char **words = hashmap_get_all_words(g_hashmap, &count);
            double elapsed = get_time_ms() - start;

            printf("{\"words\":[");
            if (words) {
                for (size_t i = 0; i < count; i++) {
                    if (i > 0) putchar(',');
                    print_json_escaped(words[i]);
                }
                hashmap_free_words(words, count);
            }
            printf("],\"count\":%zu,\"timeMs\":%.6f}\n", count, elapsed);
        } else {
            char *words[100000];
            int count = 0;
            bst_get_all_words(g_bst_root, words, &count, 100000);
            double elapsed = get_time_ms() - start;

            printf("{\"words\":[");
            for (int i = 0; i < count; i++) {
                if (i > 0) putchar(',');
                print_json_escaped(words[i]);
            }
            printf("],\"count\":%d,\"timeMs\":%.6f}\n", count, elapsed);
        }
    }
    
    fflush(stdout);
}

int main(void) {
    // Don't load anything at startup - use lazy loading for fast startup
    // Dictionary will be loaded on first use based on method requested

    char input[MAX_INPUT];
    while (fgets(input, sizeof(input), stdin)) {
        handle_command(input);
    }

    // Cleanup
    if (g_bst_loaded) bst_free(g_bst_root);
    if (g_hashmap_loaded) hashmap_destroy(g_hashmap);

    return 0;
}
