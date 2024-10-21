import time 
from selenium import webdriver
from selenium.webdriver.common.by import By
from webdriver_manager.chrome import ChromeDriverManager
import keyboard
import numpy as np

def get_maltese_guesses():
    words = []
    with open("dictionary.txt", "r") as f:
        for line in f:
            words.append(line.strip())
    return words

def get_maltese_answers():
    words = []
    with open("answers.txt", "r") as f:
        for line in f:
            words.append(line.strip())
    return words

def play(game_rows, browser, possible_guesses, possible_answers):
    # get the word list
    words = possible_guesses
    narrowed_down_list = possible_answers
    guessCount = 0

    for guess_number in range(5):
        # goal is to minimize the longest possible word list after guess & evaluation
        # start this metric at a million (we have under 100k words)
        min_wordcount = 1e6
        chosen_word = ""
        evaluation_to_wordlist_map = {}
        guessCount += 1
        
        
        if guess_number != 0:
            words_to_consider = words
        else:
            # first guess doesn't change
            # there are many "good" first guesses
            words_to_consider = ["ħarsa"]
    
        # check every word in words_to_consider to see which one gives us most information
        # (allows us to cancel out the most words)
        for word_to_guess in words_to_consider:
            temp_eval_to_words_map = {}
            
            # evaluate with every possible answer
            for possible_answer in narrowed_down_list:
                evaluation = get_evaluation(possible_answer, word_to_guess)
                        
                # store word by evaluation tuple in a list
                if tuple(evaluation) not in temp_eval_to_words_map:
                    temp_eval_to_words_map[tuple(evaluation)] = [possible_answer]
                else:
                    temp_eval_to_words_map[tuple(evaluation)].append(possible_answer)
    
    
            # metric we are trying to minimize
            biggest_possible_remaining_wordcount = max([len(val) for val in temp_eval_to_words_map.values()])
            
            # if we found a new minimum
            if biggest_possible_remaining_wordcount < min_wordcount:
                min_wordcount = biggest_possible_remaining_wordcount
                chosen_word = word_to_guess
                
                # save current best wordlist map
                evaluation_to_wordlist_map = temp_eval_to_words_map

        # evaluate chosen word with answer
        enter_guess(chosen_word)
        time.sleep(1)
        answer_evaluation = get_werdil_evaluation(chosen_word, game_rows[guess_number], browser, guessCount)
        
        if answer_evaluation in evaluation_to_wordlist_map:
            narrowed_down_list = evaluation_to_wordlist_map[answer_evaluation]
            
        if answer_evaluation == [2, 2, 2, 2, 2]:
            return [chosen_word]
        time.sleep(1)
        
        # once narrowed down to 1, we are done
        if len(narrowed_down_list) == 1:
            enter_guess(narrowed_down_list[0])
            return [chosen_word]
    return narrowed_down_list

def get_werdil_evaluation(chosen_word, game_row, browser, guessCnt):
    evaluation = []
    for letCount in range(1,6):
        ele = browser.find_element(By.XPATH, "//table/tbody/tr[{guess}]/td[{letter}]".format(guess = guessCnt, letter = letCount))
        letResult = ele.get_attribute("class")
        
        if (letResult == "green"):
            evaluation.append(2)
        elif (letResult == "orange"):
            evaluation.append(1)
        else:
            evaluation.append(0)
    return tuple(evaluation)

def enter_guess(word):
    keyboard.write(word, delay=0.05)
    keyboard.press_and_release('enter')

def get_evaluation(answer, word):
    # 0 = nothing, 1 = yellow, 2 = green
    output = [0, 0, 0, 0, 0]
    
    # check for correct letter and placement
    for i in range(5):
        if word[i] == answer[i]:
            output[i] = 2
            answer = answer[:i] + ' ' + answer[i + 1:]
           
    # check for correct letter
    for i in range(5):
        char = word[i]
        if char in answer and output[i] == 0:
            output[i] = 1
            first_occurence = answer.find(char)
            answer = answer[:first_occurence] + ' ' + answer[first_occurence + 1:]
    return tuple(output)

def main():
    start_button = 'esc'

    # set up Selenium browser
    browser = webdriver.Chrome(ChromeDriverManager().install())
    # how many rounds to play
    num_games = 100        
    browser.get("https://wordle-malti.github.io/")

    
    # wait to start the program
    keyboard.wait(start_button)
        
    for _ in range(num_games):
        # get game rows
        game_rows = np.array(browser.find_elements(By.CSS_SELECTOR, "td.guess_box.ng-binding.ng-scope"))
            
        # guess list and answer list is the same
        play(game_rows, browser, get_maltese_guesses(), get_maltese_answers())
        # play again
        time.sleep(2)
        keyboard.press('esc')
        time.sleep(2)
        keyboard.release('esc')
        time.sleep(1)
        browser.find_element(By.XPATH, '//button[text()="Ipprova kelma oħra"]').click()
        time.sleep(1)
    keyboard.wait(start_button)

if __name__ == "__main__":
    main()    