#include "question_1.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Q1 (a)
bool get_text_from_user(char *buf,size_t buf_size)
{
    printf("Please enter a paragraph of text:");
    fgets(buf, buf_size-1, stdin);
    return true;
}

bool clean_and_validate(char *str)
{
    int end = strlen(str)-1;
    if(str[end] == '\n')
        str[end] = '\0';

    if(end<1)
        return false;

    for(int i = 0; i<strlen(str);i++){
        int ascii = str[i];
        if(ascii < 32 || ascii > 126)
            return false;
    }
    return true;
}

void convert_to_lower_case(char *str,size_t len)
{
    for(size_t i = 0; i<len;i++) {
        tolower(str[i]);
    }
}

//Q1 (b)
int is_word_char(char ch)
{
    if(isalpha(ch)) {
        return 1;
    }else if(isdigit(ch)) {
        return 1;
    }else if(ch == 44 || ch == 45 || ch == 95) {
        return 1;
    }else {
        return 0;
    }
}

int is_sentence_terminator(char ch)
{
    if(ch == '.' || ch == '?' || ch == '!') {
        return 1;
    }
    else {
        return 0;
    }
}

int count_sentences(const char *text,size_t len)
{
   int count = 0;
   int current = 0;
   int previous = 0;

   for(size_t i = 0; i<len; i++){

       if(is_sentence_terminator(text[i]) == 1)
       {
           current = 1;
           if(previous == 0){
                count++;
           }

       }else{
           current = 0;
       }
       previous = current;
   }
   return count;
}

void get_char_counts(const char *text,size_t len,char_counts_t *counts)
{
    int words = 0;
    int spaces = 0;
    int others = 0;

    for(size_t i = 0; i<len; i++){
        if(is_word_char(text[i]) == 1) {
            words++;
        }
        else if(text[i] == 32){
            spaces++;
        }
        else{
            others++;
        }
    }
    counts->word = words;
    counts->space = spaces;
    counts->other = others;

}

int count_words(const char *text,size_t len)
{
    int count = 0;
    int current = 0;
    int previous = 0;

    for(size_t i = 0; i<len; i++){

        if(is_word_char(text[i]) == 0)
        {
            current = 1;
            if(previous == 0){
                count++;
            }

        }else{
            current = 0;
        }
        previous = current;
    }
    return count;
}

void get_letter_frequencies(const char *text,size_t len,int freq[26],int *max_freq)
{
    for(int i=0; i < 26; i++){
        freq[i] = 0;
    }

    int letter_place = 0;
    for(int i=0;i<len;i++){
        if(isalpha(text[i])>0){
            letter_place = text[i] - 97;
            freq[letter_place]++;
        }
    }

    int max_letter_frequency = 0;
    for(int i=0;i<26;i++){
        if(freq[i]>max_letter_frequency){
            max_letter_frequency = freq[i];
        }
    }
    *max_freq = max_letter_frequency;
}

void get_text_statistics(const char *text,size_t len,statistics_t *data)
{
    get_char_counts(text, len, &data->char_info);
    data->sentences=count_sentences(text,len);
    data->words=count_words(text,len);
    get_letter_frequencies(text,len,data->freq,&data->max_freq);

    int max_letter_count = 0;
    for(int i=0;i<26;i++){
        if(data->freq[i] == data->max_freq){
            data->most_freq_chars[max_letter_count] = i+97;
            max_letter_count++;
        }
    }
    data->most_freq_chars[max_letter_count] = '\0';
}

//Q1 (c)
bool get_text_from_file(const char *filename,char *buf,size_t buf_size)
{
    FILE *fp;
    char content[buf_size];
    fp = fopen(filename,"r");
    if (fp == NULL){
        return false;
    }
    int i = 0;
    while(fscanf(fp, "%s", content) != EOF && i < buf_size) {
        for(int j=0; j <strlen(content);j++){
            buf[i] = content[j];
            j++;
        }
        buf[i] = ' '; //to add a space between words
        i++;
    }
    fclose(fp);
    return true;
}

//Q1 (d)
bool print_menu(menu_choice_t *user_choice)
{
    printf("Menu\nChoose method of input:\n1)Command Line Input\n2)File Input\nEnter '0' to quit");

    int key = -10;
    scanf("%d", &key);
    if (key != 0 && key != 1 && key != 2) {
        *user_choice = INVALID;
        return false;
    }
    if(key == 1){
        *user_choice = USER_INPUT;
        return true;
    }
    if (key == 2){
        *user_choice = FILE_INPUT;
        return true;
    }
    if(key == 0){
        *user_choice = QUIT;
        return true;
    }
}