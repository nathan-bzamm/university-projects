# cps1011-assignment-read-me-file

## CPS1011-basecode

The following files can be found on this project:

   *cps1011-basecode*
1. *includes*
        - minunit.h
        - question_1.h
        - question_2.h
2. CMakeLists.txt
3. main_1.c
4. main_2.c
5. question_1.c
6. question_1_unit_tests.c
7. question_2.c
8. question_2_unit_tests.c

## main_1.c

This includes the main method which is used to implement the functions found in "question_1.c", and it will repeatedly present the menu to the user.  It will also clean and validate the input of the user; convert the input into lowercase characters, obtain the frequencies and statistics of the text inputted, and will then output the data in a clean and easy to read way.

## main_2.c
This includes the main method which is used to create various sparse sets in order to store the following:

    1. The declaration of two sparse sets
    2. All the characters that are used in both sparse sets
    3. All the common characters that are used in both sparse sets
    4. All the common characters that are found in the first sparse, but not in the second
    5. A function that is used to populate a sparse set from a file
    
## question_1.c

Includes the following:

1.bool get_text_from_user(char *buf,size_t buf_size)
    - retrieves text from the user and reads it into a buffer

2.bool clean_and_validate(char *str)
    - cleans and validates text

3.void convert_to_lower_case(char *str,size_t len)
    - converts all the characters in the text to lowercase
    
4.int is_word_char(char ch)
    - checks if the character that is passed is a letter, digit, hyphen, underscore, or apostrophe

5.int is_sentence_terminator(char ch) 
    - checks if a character passed is a full stop, question mark, or an exclamation mark
    
6.int count_sentences(const char *text,size_t len) 
    - counts the number of sentences
    
7.void get_char_counts(const char *text,size_t len,char_counts_t *counts)
    - counts the number of word characters, space characters, and other characters
    
8.int count_words(const char *text,size_t len)
    - counts the number of words
    
9.void get_letter_frequencies(const char *text,size_t len,int freq[26],int *max_freq)
    - counts the frequencies of the alphabetic letters in the text
    
10.void get_text_statistics(const char *text,size_t len,statistics_t *data)
    - gets the statistics of the text provided and stores them inside the statistics_t struct
    
11.bool get_text_from_file(const char *filename,char *buf,size_t buf_size)
    - opens a specified file and reads the characters in the file into a buffer
    
12.bool print_menu(menu_choice_t *user_choice) 
    - presents the user with a menu to choose whether they want to input text, choose a file to get text from, or quit the program
    
## question_1_unit_tests.c
This files contains some basic unit testing functionality, which is used to test if a particular function works or not

## question_2.c

Includes the following:

1.sparse_set_ptr ss_init(sparse_set_index_t capacity,sparse_set_elem_t max_value)
    - used to initialise a sparse set data structure
    
2.bool ss_destroy(sparse_set_ptr sparse_set)
    - destroys and cleans up the sparse set provided
    
3.bool ss_contained(sparse_set_ptr sparse_set,sparse_set_elem_t element)
    - checks whether an element is in a sparse set or not

4.bool ss_add(sparse_set_ptr sparse_set,sparse_set_elem_t element)
    - adds an element to a sparse set
    
5.bool ss_remove(sparse_set_ptr sparse_set,sparse_set_elem_t element)
    - removes an element from a sparse set
    
6.bool ss_sort(sparse_set_ptr sparse_set)
    - sorts a sparse set
    
7.ssize_t ss_size(sparse_set_ptr sparse_set)
    - returns the number of elements in a sparse set
    
8.bool ss_get_elem(sparse_set_ptr sparse_set,sparse_set_index_t sparse_index,sparse_set_elem_ptr dest)
    - retrieves the elemengt found at position 'sparse_index'
    
9.bool ss_print_status(sparse_set_ptr sparse_set,bool print_as_chars)
    - prints out the contents of the sparse set in either characters or unsigned integers
    
## question_2_unit_tests.c
This files contains some basic unit testing functionality, which is used to test if a particular function works or not

    


