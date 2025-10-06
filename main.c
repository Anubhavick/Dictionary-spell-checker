/*
 * Dictionary Spell Checker using Binary Search Tree (BST)
 *
 * Features:
 * - BST implementation where each node stores one word
 * - Load words from dictionary.txt into the BST
 * - Interactive menu to check words, add words, display all words, exit
 * - Case-insensitive comparisons; in-order traversal prints words alphabetically
 *
 * Build:
 *   make
 *
 * Run:
 *   ./spellchecker
 *
 * Author: Generated for user
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 256
#define DICT_FILE "dictionary.txt"

/*
 * BST node structure: stores a dynamically allocated lowercase word,
 * and pointers to left and right children.
 */
typedef struct BSTNode {
    char *word;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

/* Utility: create a new node with a copied word (assumed lowercased) */
BSTNode *create_node(const char *word) {
    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));
    if (!node) {
        perror("malloc");
        return NULL;
    }
    node->word = strdup(word);
    if (!node->word) {
        perror("strdup");
        free(node);
        return NULL;
    }
    node->left = node->right = NULL;
    return node;
}

/* Compare two words case-insensitively. Words are stored lowercase, so
 * a simple strcmp is sufficient for stored words. This wrapper is kept for
 * clarity and possible future changes.
 */
int word_compare(const char *a, const char *b) {
    return strcmp(a, b);
}

/* Insert a word into the BST. If the word exists, do nothing. Returns root. */
BSTNode *bst_insert(BSTNode *root, const char *word) {
    if (!root) {
        return create_node(word);
    }
    int cmp = word_compare(word, root->word);
    if (cmp == 0) {
        /* already exists */
        return root;
    } else if (cmp < 0) {
        root->left = bst_insert(root->left, word);
    } else {
        root->right = bst_insert(root->right, word);
    }
    return root;
}

/* Search for a word in the BST. Returns 1 if found, 0 otherwise. */
int bst_search(BSTNode *root, const char *word) {
    if (!root) return 0;
    int cmp = word_compare(word, root->word);
    if (cmp == 0) return 1;
    if (cmp < 0) return bst_search(root->left, word);
    return bst_search(root->right, word);
}

/* Free the whole BST recursively */
void bst_free(BSTNode *root) {
    if (!root) return;
    bst_free(root->left);
    bst_free(root->right);
    free(root->word);
    free(root);
}

/* In-order traversal: prints words in alphabetical order */
void bst_inorder_print(BSTNode *root) {
    if (!root) return;
    bst_inorder_print(root->left);
    printf("%s\n", root->word);
    bst_inorder_print(root->right);
}

/* Helper to collect and print up to 10 words that start with a given prefix (in-order) */
static void suggest_collect_prefix(BSTNode *node, const char *prefix, size_t len, int *printed) {
    if (!node || *printed >= 10) return;
    suggest_collect_prefix(node->left, prefix, len, printed);
    if (*printed >= 10) return;
    if (strncmp(node->word, prefix, len) == 0) {
        if (*printed == 0) printf("Suggestions:\n");
        printf("  %s\n", node->word);
        (*printed)++;
    }
    suggest_collect_prefix(node->right, prefix, len, printed);
}

/* Helper to print the first N words in alphabetical order (in-order) */
static void print_first_n(BSTNode *node, int *count) {
    if (!node || *count >= 10) return;
    print_first_n(node->left, count);
    if (*count >= 10) return;
    printf("  %s\n", node->word);
    (*count)++;
    print_first_n(node->right, count);
}

/* Helper: convert string to lowercase in-place */
void to_lowercase(char *s) {
    for (; *s; ++s) *s = (char)tolower((unsigned char)*s);
}

/* Load dictionary file into BST. Returns root. */
BSTNode *load_dictionary(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("fopen");
        return NULL;
    }

    char buffer[MAX_WORD_LEN];
    BSTNode *root = NULL;

    while (fgets(buffer, sizeof(buffer), f)) {
        /* strip newline and whitespace */
        char *p = buffer;
        while (*p && (unsigned char)isspace((unsigned char)*p)) ++p;
        if (*p == '\0') continue; /* skip empty lines */
        /* trim trailing whitespace */
        char *end = p + strlen(p) - 1;
        while (end > p && (unsigned char)isspace((unsigned char)*end)) *end-- = '\0';

        to_lowercase(p);
        /* insert; duplicates are handled inside bst_insert */
        root = bst_insert(root, p);
    }

    fclose(f);
    return root;
}

/* Append a word to the dictionary file. Returns 0 on success, -1 on failure. */
int append_word_to_file(const char *filename, const char *word) {
    FILE *f = fopen(filename, "a");
    if (!f) {
        perror("fopen append");
        return -1;
    }
    fprintf(f, "%s\n", word);
    fclose(f);
    return 0;
}

/* Print suggestions: simple strategy — print a few words near the alphabetic position.
 * We'll do an in-order traversal and print words that share a prefix or are close alphabetically.
 * For simplicity and to avoid storing large lists, we perform an in-order traversal and
 * print candidates when they match a prefix or are within a small lexicographic range.
 */
void suggest_words(BSTNode *root, const char *word) {
    if (!root || !word) return;
    size_t prefix_len = strlen(word);

    /* If prefix is empty, no suggestions */
    if (prefix_len == 0) return;

    int printed = 0;
    suggest_collect_prefix(root, word, prefix_len, &printed);

    if (printed == 0) {
        /* no prefix matches found — print a couple of close alphabetic neighbors */
        printf("No prefix matches. Showing nearby words alphabetically:\n");
        int cnt = 0;
        print_first_n(root, &cnt);
    }
}

/* Clean input word: remove leading/trailing whitespace, convert to lowercase, and ensure length safe */
void clean_input(char *s) {
    /* trim leading */
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    char *end = start + strlen(start) - 1;
    while (end >= start && isspace((unsigned char)*end)) *end-- = '\0';
    /* move to front if needed */
    if (start != s) memmove(s, start, strlen(start) + 1);
    to_lowercase(s);
}

/* Interactive menu loop */
void menu_loop(BSTNode **root_ptr) {
    BSTNode *root = *root_ptr;
    char choice[16];
    char buffer[MAX_WORD_LEN];

    while (1) {
        printf("\nDictionary Spell Checker\n");
        printf("1) Check spelling\n");
        printf("2) Add new word to dictionary\n");
        printf("3) Display all dictionary words (alphabetical)\n");
        printf("4) Exit\n");
        printf("Enter choice: ");
        if (!fgets(choice, sizeof(choice), stdin)) break;
        int opt = atoi(choice);

        if (opt == 1) {
            printf("Enter word to check: ");
            if (!fgets(buffer, sizeof(buffer), stdin)) break;
            clean_input(buffer);
            if (strlen(buffer) == 0) { printf("No word entered.\n"); continue; }
            if (bst_search(root, buffer)) printf("'%s' is spelled correctly.\n", buffer);
            else {
                printf("'%s' is NOT found in the dictionary.\n", buffer);
                suggest_words(root, buffer);
            }
        } else if (opt == 2) {
            printf("Enter new word to add: ");
            if (!fgets(buffer, sizeof(buffer), stdin)) break;
            clean_input(buffer);
            if (strlen(buffer) == 0) { printf("No word entered.\n"); continue; }
            if (bst_search(root, buffer)) {
                printf("'%s' already exists in the dictionary.\n", buffer);
            } else {
                root = bst_insert(root, buffer);
                if (append_word_to_file(DICT_FILE, buffer) == 0) {
                    printf("'%s' added to dictionary and saved to file.\n", buffer);
                } else {
                    printf("'%s' added to in-memory dictionary, but failed to save to file.\n", buffer);
                }
            }
        } else if (opt == 3) {
            printf("\nDictionary words (alphabetical):\n");
            bst_inorder_print(root);
        } else if (opt == 4) {
            printf("Exiting.\n");
            break;
        } else {
            printf("Invalid choice. Please enter 1-4.\n");
        }
    }

    *root_ptr = root;
}

int main(int argc, char **argv) {
    printf("Loading dictionary from '%s'...\n", DICT_FILE);
    BSTNode *root = load_dictionary(DICT_FILE);
    if (!root) {
        printf("Warning: dictionary may be empty or file missing. You can still add words.\n");
    } else {
        printf("Dictionary loaded.\n");
    }

    menu_loop(&root);

    bst_free(root);
    return 0;
}
