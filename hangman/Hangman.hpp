#include "IHangman.hpp"
#include "HangmanInterface.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm> // for algorithms like find
using namespace std;

#ifndef HANGMAN_HPP
#define HANGMAN_HPP

class Hangman : public IHangman {
private:
    const int WIDTH = 50; // sets width for centering text using setw

    // Private member functions
    
    // Takes the current guess from the user
    void TakeInput() {
        cout << endl << setw(WIDTH * 1.5) << "Enter a letter: ";
        char letter;
        cin >> letter;
        SetGuessedLetter(letter);
    }

    // Updates the word to guess
    void UpdateGuessedWord() {
        SetWordToGuess(wordlist.getRandomWord()); // Updates the current word to guess
        vector<char> new_guessed_word(GetWordToGuess().size(), '_'); // Fills the vector of guesses made so far with '_'
        SetGuessedWord(new_guessed_word);
    }

    // Processes the user's guess
    void ProcessGuess(char guess) {
        bool correct = false;
        vector<char> current_guessed_word = GetGuessedWord();
        vector<char> current_incorrect_guesses = GetIncorrectGuesses();

        // Check if the letter has already been guessed
        if (find(current_guessed_word.begin(), current_guessed_word.end(), guess) != current_guessed_word.end() ||
            find(current_incorrect_guesses.begin(), current_incorrect_guesses.end(), guess) != current_incorrect_guesses.end()) {
            cout << "You've already guessed that letter. Try another one." << endl;
            return;
        }   

        // Check if the guess is correct
        for (size_t i = 0; i < GetWordToGuess().size(); i++) {
            if (tolower(GetWordToGuess()[i]) == tolower(guess)) {
                current_guessed_word[i] = GetWordToGuess()[i];
                correct = true;
            }
        }

        SetGuessedWord(current_guessed_word);

        if (correct) {
            hangman_scorer.CorrectGuess(guess);
        } else {
            hangman_scorer.IncorrectGuess();
            vector<char> current_incorrect_guesses = GetIncorrectGuesses();
            current_incorrect_guesses.push_back(guess);
            SetIncorrectGuesses(current_incorrect_guesses);
            SetGuessesLeft(GetGuessesLeft() - 1);
        }
        SetGuessesUsed(GetGuessesUsed() + 1);
    }

    // Converts a vector of chars to a string
    string stringify(const vector<char>& vec) {
        string result;
        for (char c : vec) {
            result += c;
        }
        return result;
    }

public:
    // Constructor
    Hangman() : IHangman("words.txt") {
        hangman_interface = new HangmanInterface(this);
        UpdateGuessedWord();
    }

    // Destructor
    ~Hangman() {
        delete hangman_interface;
    }

    // Public member functions
    
    // Starts the game
    void PlayGame() {
        hangman_interface->WelcomeScreen();
    }

    // Plays a round of the game
    void PlayRound() override {
        UpdateGuessedWord();
        if (!IsGameLoaded()) {
            SetGuessesLeft(6);
            SetGuessesUsed(0);
            SetIncorrectGuesses({});
        } 
        
        while (GetGuessesLeft() > 0 && stringify(GetGuessedWord()) != GetWordToGuess()) {
            hangman_interface->GameScreen();
            TakeInput();
            ProcessGuess(GetGuessedLetter());
        }

        string guessed_str = stringify(GetGuessedWord());
        if (guessed_str == GetWordToGuess()) {
            hangman_scorer.WordGuessed(GetWordToGuess());
            SetCorrectWords(GetCorrectWords() + 1);
            cout << setw(WIDTH * 1.5) << "\nCongratulations! You guessed the word: " << GetWordToGuess() << endl;
            hangman_interface->Delay(1500);
            SaveGame();
            PlayRound();
        } else {
            cout << setw(WIDTH * 1.5) << "\nSorry, you ran out of guesses. The word was: " << GetWordToGuess() << endl;
            hangman_interface->Delay(2000);
            UpdateGuessedWord();
            hangman_interface->MainMenu();
        }
        SetGameLoaded(false);
    }

    // Creates a user profile
    void CreateProfile(const string& name) override {
        SetProfileName(name);
        SetCorrectWords(0);
        SetGuessesLeft(6);
        ofstream file(name + ".txt");
        if (file.is_open()) {
            file.close();
            cout << "Profile " << name << " created successfully!" << endl;
            cout << "\nSaving game.....\n";
            hangman_interface->Delay(2000);
            SaveGame();
        } else {
            cout << "Unable to create profile." << endl;
            hangman_interface->ProfileMenu();
        }
    }

    // Loads a user profile
    void LoadProfile(const string& name) override {
        ifstream file(name + ".txt");
        if (file.is_open()) {
            file.close();
            SetProfileName(name);
            cout << "Profile " << name << " loaded successfully!" << endl;
        } else {
            cout << "Profile " << name << " does not exist. Creating a new profile." << endl;
            hangman_interface->Delay(2000);
            CreateProfile(name);
        }
    }

    // Saves the current game state
    void SaveGame() override {
        ofstream file(GetProfileName() + ".txt");
        if (file.is_open()) {
            file << GetProfileName() << "\n"
                 << GetCorrectWords() << "\n"
                 << GetGuessesLeft() << "\n"
                 << GetScore() << "\n"
                 << GetWordToGuess() << "\n";

            for (char c : GetGuessedWord()) file << c;
            file << "\n";
            for (char c : GetIncorrectGuesses()) file << c;
            file.close();
            cout << "Game saved successfully!" << endl;
            hangman_interface->Delay(2000);
        } else {
            cout << "Unable to open file for saving." << endl;
        }
    }

    // Loads a previously saved game state
    void LoadGame() override {
        ifstream file(GetProfileName() + ".txt");
        if (file.is_open()) {
            string name;
            int correct_words, guesses_left, score;
            string word_to_guess;
            string guessed_word_str, incorrect_guesses_str;

            getline(file, name);
            file >> correct_words >> guesses_left >> score;
            file.ignore();
            getline(file, word_to_guess);
            getline(file, guessed_word_str);
            getline(file, incorrect_guesses_str);

            SetProfileName(name);
            SetCorrectWords(correct_words);
            SetGuessesLeft(guesses_left);
            SetScore(score); 
            SetWordToGuess(word_to_guess);
            SetGuessedWord(vector<char>(guessed_word_str.begin(), guessed_word_str.end()));
            SetIncorrectGuesses(vector<char>(incorrect_guesses_str.begin(), incorrect_guesses_str.end()));

            SetGameLoaded(true);
            cout << "Game loaded successfully!" << endl;
            hangman_interface->Delay(2000);
            PlayRound();
        } else {
            cout << "Unable to open file for loading." << endl;
        }
    }
};

#endif // HANGMAN_HPP