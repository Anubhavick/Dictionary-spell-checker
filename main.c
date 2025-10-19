#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 256
#define DICT_FILE "dictionary.txt"

#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"

typedef struct BSTNode {
    char *word;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

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

int word_compare(const char *a, const char *b) {
    return strcmp(a, b);
}

BSTNode *bst_insert(BSTNode *root, const char *word) {
    if (!root) {
        return create_node(word);
    }
    int cmp = word_compare(word, root->word);
    if (cmp == 0) {
        return root;
    } else if (cmp < 0) {
        root->left = bst_insert(root->left, word);
    } else {
        root->right = bst_insert(root->right, word);
    }
    return root;
}

int bst_search(BSTNode *root, const char *word) {
    if (!root) return 0;
    int cmp = word_compare(word, root->word);
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

void bst_inorder_print(BSTNode *root) {
    if (!root) return;
    bst_inorder_print(root->left);
    printf("  %s%s%s\n", COLOR_CYAN, root->word, COLOR_RESET);
    bst_inorder_print(root->right);
}

static void suggest_collect_prefix(BSTNode *node, const char *prefix, size_t len, int *printed) {
    if (!node || *printed >= 10) return;
    suggest_collect_prefix(node->left, prefix, len, printed);
    if (*printed >= 10) return;
    if (strncmp(node->word, prefix, len) == 0) {
        if (*printed == 0) printf("\n%s💡 Suggestions:%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("  %s→%s %s%s%s\n", COLOR_GREEN, COLOR_RESET, COLOR_CYAN, node->word, COLOR_RESET);
        (*printed)++;
    }
    suggest_collect_prefix(node->right, prefix, len, printed);
}


static void print_first_n(BSTNode *node, int *count) {
    if (!node || *count >= 10) return;
    print_first_n(node->left, count);
    if (*count >= 10) return;
    printf("  %s→%s %s%s%s\n", COLOR_GREEN, COLOR_RESET, COLOR_CYAN, node->word, COLOR_RESET);
    (*count)++;
    print_first_n(node->right, count);
}


void to_lowercase(char *s) {
    for (; *s; ++s) *s = (char)tolower((unsigned char)*s);
}


BSTNode *load_dictionary(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("fopen");
        return NULL;
    }

    char buffer[MAX_WORD_LEN];
    BSTNode *root = NULL;

    while (fgets(buffer, sizeof(buffer), f)) {
        
        char *p = buffer;
        while (*p && (unsigned char)isspace((unsigned char)*p)) ++p;
        if (*p == '\0') continue; 
        
        char *end = p + strlen(p) - 1;
        while (end > p && (unsigned char)isspace((unsigned char)*end)) *end-- = '\0';

        to_lowercase(p);
        
        root = bst_insert(root, p);
    }

    fclose(f);
    return root;
}


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


void suggest_words(BSTNode *root, const char *word) {
    if (!root || !word) return;
    size_t prefix_len = strlen(word);

    
    if (prefix_len == 0) return;

    int printed = 0;
    suggest_collect_prefix(root, word, prefix_len, &printed);

    if (printed == 0) {

        printf("\n%s💡 No prefix matches. Showing nearby words:%s\n", COLOR_YELLOW, COLOR_RESET);
        int cnt = 0;
        print_first_n(root, &cnt);
    }
}


void clean_input(char *s) {
    
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    char *end = start + strlen(start) - 1;
    while (end >= start && isspace((unsigned char)*end)) *end-- = '\0';
    
    if (start != s) memmove(s, start, strlen(start) + 1);
    to_lowercase(s);
}


void menu_loop(BSTNode **root_ptr) {
    BSTNode *root = *root_ptr;
    char choice[16];
    char buffer[MAX_WORD_LEN];

    while (1) {
        printf("\n%s╔════════════════════════════════════════╗%s\n", COLOR_BOLD, COLOR_RESET);
        printf("%s║   Dictionary Spell Checker          ║%s\n", COLOR_BOLD, COLOR_RESET);
        printf("%s╚════════════════════════════════════════╝%s\n", COLOR_BOLD, COLOR_RESET);
        printf("\n%s1)%s  Check spelling\n", COLOR_CYAN, COLOR_RESET);
        printf("%s2)%s  Add new word to dictionary\n", COLOR_CYAN, COLOR_RESET);
        printf("%s3)%s  Display all dictionary words\n", COLOR_CYAN, COLOR_RESET);
        printf("%s4)%s  Exit\n", COLOR_CYAN, COLOR_RESET);
        printf("\n%sEnter choice:%s ", COLOR_YELLOW, COLOR_RESET);
        if (!fgets(choice, sizeof(choice), stdin)) break;
        int opt = atoi(choice);

        if (opt == 1) {
            printf("\n%sEnter word to check:%s ", COLOR_BLUE, COLOR_RESET);
            if (!fgets(buffer, sizeof(buffer), stdin)) break;
            clean_input(buffer);
            if (strlen(buffer) == 0) { 
                printf("%s⚠️  No word entered.%s\n", COLOR_RED, COLOR_RESET); 
                continue; 
            }
            if (bst_search(root, buffer)) {
                printf("\n%s✓%s '%s%s%s' is spelled correctly! %s✓%s\n", 
                       COLOR_GREEN, COLOR_RESET, COLOR_BOLD, buffer, COLOR_RESET, COLOR_GREEN, COLOR_RESET);
            } else {
                printf("\n%s✗%s '%s%s%s' is NOT found in the dictionary.\n", 
                       COLOR_RED, COLOR_RESET, COLOR_BOLD, buffer, COLOR_RESET);
                suggest_words(root, buffer);
            }
        } else if (opt == 2) {
            printf("\n%sEnter new word to add:%s ", COLOR_BLUE, COLOR_RESET);
            if (!fgets(buffer, sizeof(buffer), stdin)) break;
            clean_input(buffer);
            if (strlen(buffer) == 0) { 
                printf("%s  No word entered.%s\n", COLOR_RED, COLOR_RESET); 
                continue; 
            }
            if (bst_search(root, buffer)) {
                printf("\n%s  '%s%s%s' already exists in the dictionary.%s\n", 
                       COLOR_YELLOW, COLOR_BOLD, buffer, COLOR_RESET, COLOR_RESET);
            } else {
                root = bst_insert(root, buffer);
                if (append_word_to_file(DICT_FILE, buffer) == 0) {
                    printf("\n%s✓%s '%s%s%s' added to dictionary and saved to file! %s✓%s\n", 
                           COLOR_GREEN, COLOR_RESET, COLOR_BOLD, buffer, COLOR_RESET, COLOR_GREEN, COLOR_RESET);
                } else {
                    printf("\n%s '%s%s%s' added to in-memory dictionary, but failed to save to file.%s\n", 
                           COLOR_YELLOW, COLOR_BOLD, buffer, COLOR_RESET, COLOR_RESET);
                }
            }
        } else if (opt == 3) {
            printf("\n%s╔════════════════════════════════════════╗%s\n", COLOR_BOLD, COLOR_RESET);
            printf("%s║    Dictionary (alphabetical)         ║%s\n", COLOR_BOLD, COLOR_RESET);
            printf("%s╚════════════════════════════════════════╝%s\n", COLOR_BOLD, COLOR_RESET);
            bst_inorder_print(root);
            printf("\n");
        } else if (opt == 4) {
            printf("\n%s Goodbye! Exiting...%s\n\n", COLOR_MAGENTA, COLOR_RESET);
            break;
        } else {
            printf("\n%s Invalid choice. Please enter 1-4.%s\n", COLOR_RED, COLOR_RESET);
        }
    }

    *root_ptr = root;
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    
    printf("\n%s╔════════════════════════════════════════╗%s\n", COLOR_BOLD, COLOR_RESET);
    printf("%s║    Dictionary Spell Checker          ║%s\n", COLOR_BOLD, COLOR_RESET);
    printf("%s╚════════════════════════════════════════╝%s\n", COLOR_BOLD, COLOR_RESET);
    printf("\n%sLoading dictionary from '%s'...%s\n", COLOR_CYAN, DICT_FILE, COLOR_RESET);
    
    BSTNode *root = load_dictionary(DICT_FILE);
    if (!root) {
        printf("%s  Warning: dictionary may be empty or file missing.%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%s   You can still add words.%s\n", COLOR_YELLOW, COLOR_RESET);
    } else {
        printf("%s✓ Dictionary loaded successfully!%s\n", COLOR_GREEN, COLOR_RESET);
    }

    menu_loop(&root);

    bst_free(root);
    return 0;
}
