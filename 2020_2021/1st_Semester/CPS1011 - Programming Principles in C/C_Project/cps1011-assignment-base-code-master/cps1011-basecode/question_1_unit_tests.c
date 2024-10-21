#include <stdio.h>
#include <string.h>

#include <minunit.h>

#include "question_1.h"

char corpus[] = "The quick brown fox jumps over the lazy dog.";
char corpus_2_sentences[] = "The quick brown fox jumps over the lazy dog. The quick brown fox jumps over the lazy dog.";

MU_CUSTOM_TEST_START(testCleanAndValidate)
    char text[] = "The quick brown fox jumps over the lazy dog.\n";
    int count = (int)strlen(text);
    bool result = clean_and_validate(text);
    mu_custom_check(result, "expected function to return true", 1, 2);
    mu_custom_check(text[count - 1] == '\0', "expected last character to be NULL",  2, 2);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testConvertTolower)
    convert_to_lower_case(corpus, strlen(corpus));
    
    int index = 1;
    for(char *char_ptr = corpus; *char_ptr; char_ptr++, index++)
    {
        mu_custom_check(*char_ptr < 'A' || *char_ptr > 'Z', "expected lowercase chars only", index, strlen(corpus));
    }
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testIsSentenceTerminator_1) // To do: Why "_1"?
    mu_custom_check(!is_sentence_terminator(','), "',' is not a sentence terminator", 1, 2);
    mu_custom_check(is_sentence_terminator('?'), "'?' is a sentence terminator", 2, 2);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testCountSentences)
    printf("Text: \"%s\"\n", corpus_2_sentences);
    int sentence_count = count_sentences(corpus_2_sentences, strlen(corpus_2_sentences));
    mu_custom_check(sentence_count == 2, "expected 2 sentences", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testIsWordChar_1) // To do: Why "_1"?
    mu_custom_check(!is_word_char(' '), "' ' is not a word char", 1, 2);
    mu_custom_check(is_word_char('a'), "'a' is a word char", 2, 2);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testGetCharCount)
    printf("Text: \"%s\"\n", corpus);
    char_counts_t stats;
    get_char_counts(corpus, strlen(corpus), &stats);
    mu_custom_check(stats.space == 8, "expected 8 space chars", 1, 3);
    mu_custom_check(stats.word == 35, "expected 35 word chars", 2, 3);
    mu_custom_check(stats.other == 1, "expected 1 'other' char", 3, 3);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testCountWords)
    printf("Text: \"%s\"\n", corpus);
    int word_count = count_words(corpus, strlen(corpus));
    mu_custom_check(word_count == 9, "expected 9 words", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testGetLetterFrequencies)
    printf("Text: \"%s\"\n", corpus);
    int freq[26];
    int max_freq;
    convert_to_lower_case(corpus, strlen(corpus));
    get_letter_frequencies(corpus, strlen(corpus), freq, &max_freq);

    for(int index = 0; index < 26; index++)
    {
        mu_check(freq[index] >= 1); // To do: Check this!
    }

    mu_custom_check(max_freq == 4, "expected max frequency 4", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testGetTextStatistics)
    printf("Text: \"%s\"\n", corpus);
    int freq[26];
    int max;
    convert_to_lower_case(corpus, strlen(corpus));
    statistics_t details;
    get_text_statistics(corpus, strlen(corpus), &details);

    mu_custom_check(details.sentences == 1, "expected sentences 1",  1, 6);
    mu_custom_check(details.words == 9,  "expected words 9", 2, 6);
    mu_custom_check(details.char_info.space == 8, "expected spaces 8",  3, 6);
    mu_custom_check(details.max_freq == 4, "expected max frequency 4", 4, 6);
    mu_custom_check(strlen(details.most_freq_chars) == 1, "expected num most freq chars 1", 5, 6);
    mu_custom_check(details.most_freq_chars[0] == 'o', "expected 'o' to be most frequent", 6, 6);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testGetTextFromFile)
    char text_buffer[1000];
    char file_path[] = "./text.txt";
    bool found = get_text_from_file(file_path, text_buffer, 1000);
    mu_custom_check(!found, "unexpected text file found", 1, 1);
MU_CUSTOM_TEST_END

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(testCleanAndValidate);
    MU_RUN_TEST(testConvertTolower);
    MU_RUN_TEST(testIsSentenceTerminator_1);
    MU_RUN_TEST(testCountSentences);
    MU_RUN_TEST(testIsWordChar_1);
    MU_RUN_TEST(testCountWords);
    MU_RUN_TEST(testGetCharCount);
    MU_RUN_TEST(testGetLetterFrequencies);
    MU_RUN_TEST(testGetTextStatistics);
    MU_RUN_TEST(testGetTextFromFile);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
