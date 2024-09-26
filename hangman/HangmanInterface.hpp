/*
This class handles the game's interface
it displays a welcome screen, the profile menu (to select profile);
a main menu to start, load or save game
and a game screen to guess a given word
*/

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm> // for algorithms like find
#include <chrono> // for timing
#include <windows.h> // windows specific functionalities; setting text color
#include "IHangman.hpp"
using namespace std;

#ifndef HANGMAN_INTERFACE_HPP
#define HANGMAN_INTERFACE_HPP


class HangmanInterface {
private:
    const int WIDTH = 50; // sets the width for centering text using setw
    IHangman* hangman; // a pointer to IHangman object, the abstractt base class

    // Method to display the keyboard
    void DisplayKeyboard(const vector<char>& guessed, const vector<char>& incorrect_guesses) {
        string upper_keys = "QWERTYUIOP";
        string middle_keys = "ASDFGHJKL";
        string lower_keys = "ZXCVBNM";

        cout << "\n\n\n\n\n\n" << setw(WIDTH) << "";

        // Display upper row keys
        for (char key : upper_keys) {
            if (find(guessed.begin(), guessed.end(), tolower(key)) != guessed.end()) {
                SetColor(2); // Green for correct guess
            } else if (find(incorrect_guesses.begin(), incorrect_guesses.end(), tolower(key)) != incorrect_guesses.end()) {
                SetColor(4); // Red for incorrect guess
            } else {
                SetColor(8); // Gray for non-guessed letters
            }
            cout << key << "\t";
        }
        cout << "\n\n" << setw(WIDTH) << "";

        // Display middle row keys with some padding at the start
        cout << "   ";
        for (char key : middle_keys) {
            if (find(guessed.begin(), guessed.end(), tolower(key)) != guessed.end()) {
                SetColor(2);
            } else if (find(incorrect_guesses.begin(), incorrect_guesses.end(), tolower(key)) != incorrect_guesses.end()) {
                SetColor(4);
            } else {
                SetColor(8);
            }
            cout << key << "\t";
        }
        cout << "\n\n" << setw(WIDTH) << "";

        // Display lower row keys with more padding at the start
        cout << "\t";
        for (char key : lower_keys) {
            if (find(guessed.begin(), guessed.end(), tolower(key)) != guessed.end()) {
                SetColor(2);
            } else if (find(incorrect_guesses.begin(), incorrect_guesses.end(), tolower(key)) != incorrect_guesses.end()) {
                SetColor(4);
            } else {
                SetColor(8);
            }
            cout << key << "\t";
        }
        cout << "\n\n";
        SetColor(7); // Reset to white color
    }

    // Method to select text color
    void SetColor(int color) {SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);}

public:
    // constructor to initialize the Ihangman ppointer
    HangmanInterface(IHangman* hangman_ptr): hangman(hangman_ptr) {}

   void Delay(int milliseconds) {
        Sleep(milliseconds);
    }

    // display a welcome screen onces the game starts
    void WelcomeScreen() {
        system("cls"); // clears the console
        cout
            << setw(WIDTH * 1.5) <<"....Welcome to Hangman!" << endl
            << setw(WIDTH * 1.5) <<"Press Enter to start..." << endl;
        cin.ignore();
        Delay(1000); // 1-second delay
        ProfileMenu(); // displays the profile menu
    }

    // Method to select user profile
    void ProfileMenu() {

        // checks if escaped has been pressed
        while (true) {
            system("cls");
            cout
                << "\n\n"
                << "\tPress ESC to exit...\n\n"
                << setw(WIDTH * 1.5) << "1. Select Profile\n"
                << setw(WIDTH * 1.5) << "2. Create Profile\n";

            // gets the user's choice
            int choice;
            cin >> choice;

            if (choice == 1) {
                cout << "Enter profile name: ";
                string name;
                cin >> name;
                hangman->LoadProfile(name);
                break;
            } else if (choice == 2) {
                cout << "Enter new profile name: ";
                string name;
                cin >> name;
                hangman->CreateProfile(name);
                break;
            }
        }
        Delay(1000); // 1-second delay
        MainMenu(); // displays the main menu afterwards
    }

    // Method to start a new game, load previous one or save the current one
    void MainMenu() {
        system("cls");
        cout << "\n\n\n\n"
            << setw(WIDTH * 1.5) << "1. New Game \n"
            << setw(WIDTH * 1.5) << "2. Load Game\n";
            // << setw(WIDTH * 1.5) << "3. Save Game\n";

        // gets the user's choice
        int choice;
        cin >> choice;

        if (choice == 1) { // New Game
            hangman->PlayRound();
        } else if (choice == 2) { // Load Game
            hangman->LoadGame();
        } else if (choice == 3) { // Save Game
            hangman->SaveGame();
        } else {
            MainMenu(); // displays the main menu again if none of the choices is selected
        }
    }

    // Method that displays the actual game screen: word to guess, keyboard, score, user profile, etc.
    void GameScreen() {
        system("cls");
        // displays the game's header
        cout
            <<"\n\n\n\n"
            << hangman->GetWordToGuess() << endl
            << setw(WIDTH / 1.5) << "Profile: " << hangman->GetProfileName()
            << setw(WIDTH / 1.5) << "Score: " << hangman->GetScore()
            << setw(WIDTH / 1.5) << "Guesses left: " << hangman->GetGuessesLeft()
            << setw(WIDTH / 1.5) << "Correct Words: " << hangman->GetCorrectWords()
            << "\n\n\n\n";

        // displays the current word
        cout << "\n\n" << setw(WIDTH * 1.5) << "" << "Word: ";
        for (auto letter : hangman->GetGuessedWord())
            cout << letter << " ";
        DisplayKeyboard(hangman->GetGuessedWord(), hangman->GetIncorrectGuesses());
    }

};

#endif // HANGMAN_INTERFACE_HPP
