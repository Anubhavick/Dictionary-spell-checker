#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define DICT_FILE "dictionary.txt"

// Utility functions
void to_lowercase(char *str);
int compare_words(const void *a, const void *b);
void load_dictionary(char ***words, int *count);
void save_dictionary_sorted(char **words, int count);
void free_dictionary(char **words, int count);

#endif
