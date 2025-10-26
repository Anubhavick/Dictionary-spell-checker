#include "utils.h"

// Convert string to lowercase
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Compare function for qsort
int compare_words(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

// Load dictionary from file
void load_dictionary(char ***words, int *count) {
    FILE *file = fopen(DICT_FILE, "r");
    if (!file) {
        fprintf(stderr, "Warning: Could not open dictionary file\n");
        *words = NULL;
        *count = 0;
        return;
    }

    char buffer[MAX_WORD_LENGTH];
    int capacity = 10000;
    *words = (char **)malloc(capacity * sizeof(char *));
    if (!*words) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        *count = 0;
        return;
    }
    *count = 0;

    while (fgets(buffer, MAX_WORD_LENGTH, file)) {
        // Remove newline and carriage return
        buffer[strcspn(buffer, "\r\n")] = 0;
        
        if (strlen(buffer) == 0) continue;

        if (*count >= capacity) {
            capacity *= 2;
            char **temp = (char **)realloc(*words, capacity * sizeof(char *));
            if (!temp) {
                fprintf(stderr, "Error: Memory reallocation failed\n");
                free_dictionary(*words, *count);
                fclose(file);
                *words = NULL;
                *count = 0;
                return;
            }
            *words = temp;
        }

        (*words)[*count] = (char *)malloc(strlen(buffer) + 1);
        if (!(*words)[*count]) {
            fprintf(stderr, "Error: Memory allocation failed for word\n");
            free_dictionary(*words, *count);
            fclose(file);
            *words = NULL;
            *count = 0;
            return;
        }
        strcpy((*words)[*count], buffer);
        (*count)++;
    }

    fclose(file);
}

// Save dictionary to file in sorted order
void save_dictionary_sorted(char **words, int count) {
    // Sort the words
    qsort(words, count, sizeof(char *), compare_words);

    FILE *file = fopen(DICT_FILE, "w");
    if (!file) {
        fprintf(stderr, "Error: Cannot write to dictionary file\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n", words[i]);
    }

    fclose(file);
}

// Free dictionary memory
void free_dictionary(char **words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}
