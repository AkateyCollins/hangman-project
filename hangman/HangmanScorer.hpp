/*
this class is used to score the hangman game
+10 for correctly guessing a letter, -5 for wrong guess
word length * 5 for correctly guessing the word,
unused guesses * 10 and extra 50
*/

#include <string>
using namespace std;


#ifndef HangmanScorer_HPP
#define HangmanScorer_HPP
class HangmanScorer {
private:
    int points; // total points / score accumulated
    int correct_guesses; // number of correct guesses
    int incorrect_guesses; // number of incorrect guesses
    int correct_words; // number of correctly guessed words

public:
    // Constructor to initialize the points and guesses
    HangmanScorer(int initial_points = 0)
        : points(initial_points), correct_guesses(0), incorrect_guesses(0), correct_words(0) {}

    // Method to handle a correct guess
    void CorrectGuess(char letter) {
        points += 10;
        correct_guesses++;
    }
    
    // Method to handle an incorrect guess
    void IncorrectGuess() {
        points -= 5;
        incorrect_guesses++;
    }

    // Method to handle the word being guessed
    void WordGuessed(const string& word) {
        int wordLengthBonus = word.length() * 5; // score for correctly guessing a word
        points += wordLengthBonus; 

        int unusedGuessesBonus = (6 - incorrect_guesses) * 10; // unused guessed bonus
        points += unusedGuessesBonus;

        points += 50;  // Winning bonus
    }
    
    // Method to get the current score
    int GetScore() const {return points;}
    
    // Method to get the number of correctly guessed words
    int GetCorrectWords() const {return correct_words;}

    // Method to set the score / points
    void SetScore(int score) {this->points = score;}
};

#endif