#include <stdio.h>

#include "question_1.h"
#include <string.h>

#define BUFFER_SIZE 999
#define  ALPHABET_LETTERS 26

int main(void)
{
    //Creating instance to enum
    menu_choice_t user_Choice;
    //Creating pointer to struct
    statistics_t stats;

    //text buffer to store paragraph input
    char buffer[BUFFER_SIZE];

    //char used to take name of input file from user
    char file_name[BUFFER_SIZE];

    int letter_frequency[ALPHABET_LETTERS];//int array used to store the frequency of each letter
    int max_frequency = 0;//The highest letter frequency

    int total_characters = 0; //The total amount of characters

    bool quit_flag =false;
    while(!quit_flag){
        print_menu(&user_Choice);//Calling the function used to print the menu
        if(user_Choice == USER_INPUT || user_Choice == FILE_INPUT){
            fflush(stdin);

            if(user_Choice == USER_INPUT){
                printf("Please enter the user input\n");
                if(get_text_from_user(buffer,BUFFER_SIZE) != true){
                    fprintf(stderr, "Error occurred whilst getting user input");
                    return 1;
                }
                clean_and_validate(buffer);
            } else if(user_Choice == FILE_INPUT){
                printf("Please enter file name\n");
                gets(file_name);
                for(int i=0; i<BUFFER_SIZE; i++){
                    if(file_name[i] == '\n' || file_name[i] == '\0'){
                        file_name[i] = 0;
                    }
                } if(get_text_from_file(file_name, buffer, BUFFER_SIZE)!= true){
                    fprintf(stderr,"Error in obtaining file input for paragraph buffer\n");
                    return 1;
                }
                clean_and_validate(buffer);
            }

            convert_to_lower_case(buffer, strlen(buffer));

            printf("\n");
            printf(buffer, "\n");
            get_letter_frequencies(buffer, strlen(buffer), letter_frequency, &max_frequency);

            printf("Letter Frequencies");
            for(int i=0;i<ALPHABET_LETTERS; i++){
                if(letter_frequency[i] != 0){
                    printf("[%c] :%3d\n", (i+97), letter_frequency[i]);
                }
            }
            get_text_statistics(buffer, strlen(buffer), &stats);
            total_characters = stats.char_info.space + stats.char_info.other + stats.char_info.word;
            printf("\n\n");

            printf("+--------------------------------+-------+\n");
            printf("| Feature                        | Value |\n");
            printf("+--------------------------------+-------+\n");
            printf("|Number of characters            |%-7d|\n", total_characters);
            printf("Number of word characters        |%-7d|\n", stats.char_info.word);
            printf("Number of spaces                 |%-7d|\n", stats.char_info.space);
            printf("Number of other characters       |%-7d|\n", stats.char_info.other);
            printf("Number of words                  |%-7d|\n", stats.words);
            printf("Number of sentences              |%-7d|\n", stats.sentences);
            printf("Maximum frequency                |%-7d|\n", stats.max_freq);
            printf("Most frequent letter(s)          |   [%c] |\n", stats.most_freq_chars[0]);
            for(int i=1; stats.most_freq_chars[i] != '\0'; i++){//use of a loop to print the most frequent letters if there ever are multiple letters
                printf("|                                |   [%c] |\n", stats.most_freq_chars[i]);
            }
            printf("+--------------------------------+-------+\n\n\n");
        } else if(user_Choice == QUIT){
            quit_flag = true;
        }
    }
}



