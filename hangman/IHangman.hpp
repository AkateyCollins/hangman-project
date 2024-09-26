/*
This is the abstract base class; It would be inherited by the Hangman class
The class has member variables the hold the current game's state like the game's list of words,
the current word to guess, number of guesse left, etc.
*/

#ifndef IHANGMAN_HPP
#define IHANGMAN_HPP

#include <string>
#include <vector>
#include "WordList.hpp"
#include "HangmanScorer.hpp"
using namespace std;
class HangmanInterface;

class IHangman {
protected:
    // Member variables
    WordList wordlist; // list of words for the game
    string word_to_guess; // the current word to guess
    char guessed_letter; // the currently guessed letter
    vector<char> guessed_word; // the correct guesses so far, for a given word
    vector<char> incorrect_guesses; // the incorrect guesses made so far
    int guesses_left; //  number of guesses left, for a given word
    int guesses_used; // number of guesses used, for a given word
    HangmanScorer hangman_scorer; // an object that holds methods for scoring
    HangmanInterface* hangman_interface; // an object that provides the gaming interface
    string profile_name; // the current player's name
    int correct_words; // the number of correctly guesses words
    bool is_game_loaded; // checks if game is loaded or is new

public:
    IHangman(const string& word_file) :
        wordlist(word_file),
        guesses_left(6),
        guesses_used(0),
        correct_words(0),
        is_game_loaded(false) {}

    virtual ~IHangman() = default;

    // Pure virtual functions
    virtual void CreateProfile(const std::string& name) = 0; // creates a user profile
    virtual void LoadProfile(const std::string& name) = 0; // load a user profile if it exists or it creates a new one
    virtual void SaveGame() = 0; // save the current game for the current user
    virtual void LoadGame() = 0; // loads the current users previous game
    virtual void PlayRound() = 0; // plays a single round of hangman

    // Getters

    virtual string GetWordToGuess() const { return word_to_guess; }
    virtual vector<char> GetGuessedWord() const { return guessed_word; }
    virtual vector<char> GetIncorrectGuesses() const { return incorrect_guesses; }
    virtual char GetGuessedLetter() const { return guessed_letter; }
    virtual int GetGuessesLeft() const { return guesses_left; }
    virtual int GetScore() const { return hangman_scorer.GetScore(); }
    virtual int GetCorrectWords() const { return correct_words; }
    virtual bool IsGameLoaded() const { return is_game_loaded; }
    virtual int GetGuessesUsed() const { return guesses_used; }

    // Setters
    virtual void SetProfileName(const std::string& name) { profile_name = name; }
    virtual void SetWordToGuess(const std::string& word) { word_to_guess = word; }
    virtual void SetGuessedWord(const std::vector<char>& word) { guessed_word = word; }
    virtual void SetIncorrectGuesses(const std::vector<char>& guesses) { incorrect_guesses = guesses; }
    virtual void SetGuessedLetter(char letter) { guessed_letter = letter; }
    virtual void SetGuessesLeft(int guesses) { guesses_left = guesses; }
    virtual void SetScore(int score) { hangman_scorer.SetScore(score); }
    virtual void SetCorrectWords(int words) { correct_words = words; }
    virtual void SetGameLoaded(bool loaded) { is_game_loaded = loaded; }
    virtual void SetGuessesUsed(int guesses) { guesses_used = guesses; }
};
#endif // IHANGMAN_HPP
