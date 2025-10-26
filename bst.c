#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

typedef struct Node {
    char word[MAX_WORD_LENGTH];
    struct Node *left;
    struct Node *right;
} Node;

// Create new node
Node* create_node(const char *word) {
    Node *node = (Node *)malloc(sizeof(Node));
    strcpy(node->word, word);
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Insert into BST
Node* insert(Node *root, const char *word) {
    if (root == NULL) {
        return create_node(word);
    }

    int cmp = strcmp(word, root->word);
    if (cmp < 0) {
        root->left = insert(root->left, word);
    } else if (cmp > 0) {
        root->right = insert(root->right, word);
    }

    return root;
}

// Search in BST
int search(Node *root, const char *word) {
    if (root == NULL) {
        return 0;
    }

    int cmp = strcmp(word, root->word);
    if (cmp == 0) {
        return 1;
    } else if (cmp < 0) {
        return search(root->left, word);
    } else {
        return search(root->right, word);
    }
}

// Inorder traversal to get sorted list
void inorder(Node *root, char **words, int *index) {
    if (root == NULL) return;

    inorder(root->left, words, index);
    words[*index] = (char *)malloc(strlen(root->word) + 1);
    strcpy(words[*index], root->word);
    (*index)++;
    inorder(root->right, words, index);
}

// Load dictionary into BST
Node* load_bst(int *word_count) {
    char **words;
    int count;
    load_dictionary(&words, &count);

    Node *root = NULL;
    for (int i = 0; i < count; i++) {
        root = insert(root, words[i]);
    }

    *word_count = count;
    free_dictionary(words, count);
    return root;
}

// Count nodes in BST
int count_nodes(Node *root) {
    if (root == NULL) return 0;
    return 1 + count_nodes(root->left) + count_nodes(root->right);
}

// Free BST
void free_bst(Node *root) {
    if (root == NULL) return;
    free_bst(root->left);
    free_bst(root->right);
    free(root);
}

// Main function
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./bst [check|add|list] <word>\n");
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

        int word_count;
        Node *root = load_bst(&word_count);

        if (search(root, word)) {
            printf("FOUND: '%s'\n", word);
        } else {
            printf("NOT_FOUND: '%s'\n", word);
        }

        free_bst(root);

    } else if (strcmp(operation, "add") == 0) {
        if (argc < 3) {
            printf("Error: Word required for add operation\n");
            return 1;
        }

        char word[MAX_WORD_LENGTH];
        strcpy(word, argv[2]);
        to_lowercase(word);

        int word_count;
        Node *root = load_bst(&word_count);

        if (search(root, word)) {
            printf("EXISTS: '%s'\n", word);
            free_bst(root);
            return 0;
        }

        // Add word to BST
        root = insert(root, word);

        // Convert BST to sorted array
        int total_count = count_nodes(root);
        char **words = (char **)malloc(total_count * sizeof(char *));
        int index = 0;
        inorder(root, words, &index);

        // Save to file
        save_dictionary_sorted(words, total_count);

        printf("ADDED: '%s'\n", word);

        free_dictionary(words, total_count);
        free_bst(root);

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
