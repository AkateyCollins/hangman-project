#include <iostream>
#include <string>
#include <set>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

// Base class for Player
class Player {
protected:
    string name;
    int score;
public:
    // Constructor
    Player(const string &player_name) : name(player_name), score(0) {}

    // Getter functions
    string get_name() { return name; }
    int get_score() { return score; }
    void increment_score() { ++score; }

    // Setter functions
    void set_name(const string &player_name) { name = player_name; }
    void set_score(int new_score) { score = new_score; }

    // Save player profile to file
    virtual void save_profile(const string &filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            file << name << endl;
            file << score << endl;
            file.close();
        } else {
            cerr << "Unable to open file for saving profile." << endl;
        }
    }

    // Load player profile from file
    virtual void load_profile(const string &filename) {
        ifstream file(filename);
        if (file.is_open()) {
            getline(file, name);
            file >> score;
            file.close();
        } else {
            cerr << "Unable to open file for loading profile." << endl;
        }
    }

    // Destructor
    virtual ~Player() {}
};

// Derived class for HangmanPlayer
class HangmanPlayer : public Player {
public:
    // Constructor
    HangmanPlayer(const string &player_name) : Player(player_name) {}
};

// Base class for Game
class Game {
public:
    virtual void start() = 0;
    virtual void end() = 0;
    virtual void save_progress(const std::string &filename) const = 0;
    virtual void load_progress(const std::string &filename) = 0;

    // Destructor
    virtual ~Game() {}
};

// Derived class for Hangman
class Hangman : public Game {
private:
    HangmanPlayer &player;
    string word_to_guess;
    string guessed_word;
    int max_attempts;
    int current_attempts;
    set<char> guessed_letters;

public:
    // Constructor
    Hangman(HangmanPlayer &player, const vector<string> &words, int attempts)
        : player(player), max_attempts(attempts), current_attempts(0) {
        if (words.empty()) {
            throw invalid_argument("The list of words cannot be empty.");
        }

        // Select a random word from the list
        mt19937 rng(time(nullptr));
        uniform_int_distribution<int> dist(0, words.size() - 1);
        word_to_guess = words[dist(rng)];
        guessed_word = string(word_to_guess.size(), '_');
    }

    // Display the current guessed word
    void display_word() {
        for (char c : guessed_word) {
            cout << c << ' ';
        }
        cout << endl;
    }

    // Handle guessing a letter
    bool guess_letter(char letter) {
        if (guessed_letters.find(letter) != guessed_letters.end()) {
            cout << "Letter already guessed!" << endl;
            return false;
        }

        guessed_letters.insert(letter);
        bool correct_guess = false;

        for (size_t i = 0; i < word_to_guess.size(); ++i) {
            if (word_to_guess[i] == letter) {
                guessed_word[i] = letter;
                correct_guess = true;
            }
        }

        if (!correct_guess) {
            ++current_attempts;
        }

        return correct_guess;
    }

    // Check if the game is won
    bool is_won() const {
        return guessed_word == word_to_guess;
    }

    // Check if the game is lost
    bool is_lost() const {
        return current_attempts >= max_attempts;
    }

    // Display the game status
    void display_status() const {
        cout << "Guessed letters: ";
        for (char c : guessed_letters) {
            cout << c << ' ';
        }
        cout << endl;

        cout << "Attempts remaining: " << (max_attempts - current_attempts) << endl;
    }

    // Start the game
    void start() override {
        while (!is_won() && !is_lost()) {
            display_word();
            display_status();

            char guess;
            cout << "Enter a letter to guess: ";
            cin >> guess;

            if (!guess_letter(guess)) {
                cout << "Incorrect guess!" << endl;
            }

            cout << endl;
        }

        if (is_won()) {
            cout << "Congratulations! You guessed the word: " << word_to_guess << endl;
            player.increment_score();
        } else {
            cout << "You lost! The word was: " << word_to_guess << endl;
        }

        player.save_profile("player_profile.txt");
        save_progress("hangman_save.txt");
    }

    // End the game
    void end() override {
        cout << "Game over. Player " << player.get_name() << " scored: " << player.get_score() << endl;
    }

    // Save game progress to file
    void save_progress(const string &filename) const override {
        ofstream file(filename);
        if (file.is_open()) {
            file << player.get_name() << endl;
            file << player.get_score() << endl;
            file << word_to_guess << endl;
            file << guessed_word << endl;
            file << max_attempts << endl;
            file << current_attempts << endl;
            for (char c : guessed_letters) {
                file << c << ' ';
            }
            file << endl;
            file.close();
        } else {
            cerr << "Unable to open file for saving progress." << endl;
        }
    }

    // Load game progress from file
    void load_progress(const string &filename) override {
        ifstream file(filename);
        if (file.is_open()) {
            string temp_name;
            getline(file, temp_name);
            player.set_name(temp_name);
            int temp_score;
            file >> temp_score;
            player.set_score(temp_score);
            file.ignore();  // Ignore the newline character after the score

            getline(file, word_to_guess);
            getline(file, guessed_word);
            file >> max_attempts;
            file >> current_attempts;

            guessed_letters.clear();
            char letter;
            while (file >> letter) {
                guessed_letters.insert(letter);
            }
            file.close();
        } else {
            cerr << "Unable to open file for loading progress." << endl;
        }
    }
};

int main() {
    string player_name;
    cout << "Enter player name: ";
    cin >> player_name;

    HangmanPlayer player(player_name);

    // List of words to choose from
    vector<string> words = {"example", "hangman", "programming", "openai", "cplusplus"};

    Hangman game(player, words, 6);

    game.start();
    game.end();

    return 0;
}
