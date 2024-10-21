#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct char_counts
{
    int word;
    int space;
    int other;
} char_counts_t;

typedef struct statistics
{
    char_counts_t char_info;
    int sentences;
    int words;
    int freq[26];
    int max_freq;
    char most_freq_chars[27];
} statistics_t;

typedef enum menu_choice
{
    USER_INPUT,
    FILE_INPUT,
    QUIT,
    INVALID
} menu_choice_t;

// Q1(a)

bool get_text_from_user(char *buf, size_t buf_size);

bool clean_and_validate(char *str);

void convert_to_lower_case(char *str, size_t len);

// Q1(b)

int is_sentence_terminator(char ch);

int count_sentences(const char *text, size_t len);

int is_word_char(char ch);

int count_words(const char *text, size_t len);

void get_char_counts(const char *text, size_t len, char_counts_t *counts);

void get_letter_frequencies(const char *text, size_t len, int freq[26], int *max_freq);

void get_text_statistics(const char *text, size_t len, statistics_t *data);

bool get_text_from_file(const char* file_path, char *buf, size_t buf_size);

bool print_menu(menu_choice_t *user_choice);
