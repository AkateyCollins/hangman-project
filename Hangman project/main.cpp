#include <iostream>
#include <string>
#include <set>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>

// Base class for Player
class Player {
protected:
    std::string name;
    int score;

public:
    Player(const std::string &player_name) : name(player_name), score(0) {}

    std::string get_name() const { return name; }
    int get_score() const { return score; }
    void increment_score() { ++score; }

    void set_name(const std::string &player_name) { name = player_name; }
    void set_score(int new_score) { score = new_score; }

    virtual void save_profile(const std::string &filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << name << std::endl;
            file << score << std::endl;
            file.close();
        } else {
            std::cerr << "Unable to open file for saving profile." << std::endl;
        }
    }

    virtual void load_profile(const std::string &filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::getline(file, name);
            file >> score;
            file.close();
        } else {
            std::cerr << "Unable to open file for loading profile." << std::endl;
        }
    }

    virtual ~Player() {}
};

// Derived class for HangmanPlayer
class HangmanPlayer : public Player {
public:
    HangmanPlayer(const std::string &player_name) : Player(player_name) {}
};

// Base class for Game
class Game {
public:
    virtual void start() = 0;
    virtual void end() = 0;
    virtual void save_progress(const std::string &filename) const = 0;
    virtual void load_progress(const std::string &filename) = 0;
    virtual ~Game() {}
};

// Derived class for Hangman
class Hangman : public Game {
private:
    HangmanPlayer &player;
    std::string word_to_guess;
    std::string guessed_word;
    int max_attempts;
    int current_attempts;
    std::set<char> guessed_letters;

public:
    Hangman(HangmanPlayer &player, const std::vector<std::string> &words, int attempts)
        : player(player), max_attempts(attempts), current_attempts(0) {
        if (words.empty()) {
            throw std::invalid_argument("The list of words cannot be empty.");
        }
        // Select a random word from the list
        std::mt19937 rng(std::time(nullptr));
        std::uniform_int_distribution<int> dist(0, words.size() - 1);
        word_to_guess = words[dist(rng)];

        guessed_word = std::string(word_to_guess.size(), '_');
    }

    void display_word() const {
        for (char c : guessed_word) {
            std::cout << c << ' ';
        }
        std::cout << std::endl;
    }

    bool guess_letter(char letter) {
        if (guessed_letters.find(letter) != guessed_letters.end()) {
            std::cout << "Letter already guessed!" << std::endl;
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

    bool is_won() const {
        return guessed_word == word_to_guess;
    }

    bool is_lost() const {
        return current_attempts >= max_attempts;
    }

    void display_status() const {
        std::cout << "Guessed letters: ";
        for (char c : guessed_letters) {
            std::cout << c << ' ';
        }
        std::cout << std::endl;

        std::cout << "Attempts remaining: " << (max_attempts - current_attempts) << std::endl;
    }

    void start() override {
        while (!is_won() && !is_lost()) {
            display_word();
            display_status();

            char guess;
            std::cout << "Enter a letter to guess: ";
            std::cin >> guess;

            if (!guess_letter(guess)) {
                std::cout << "Incorrect guess!" << std::endl;
            }

            std::cout << std::endl;
        }

        if (is_won()) {
            std::cout << "Congratulations! You guessed the word: " << word_to_guess << std::endl;
            player.increment_score();
        } else {
            std::cout << "You lost! The word was: " << word_to_guess << std::endl;
        }

        player.save_profile("player_profile.txt");
        save_progress("hangman_save.txt");
    }

    void end() override {
        std::cout << "Game over. Player " << player.get_name() << " scored: " << player.get_score() << std::endl;
    }

    void save_progress(const std::string &filename) const override {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << player.get_name() << std::endl;
            file << player.get_score() << std::endl;
            file << word_to_guess << std::endl;
            file << guessed_word << std::endl;
            file << max_attempts << std::endl;
            file << current_attempts << std::endl;
            for (char c : guessed_letters) {
                file << c << ' ';
            }
            file << std::endl;
            file.close();
        } else {
            std::cerr << "Unable to open file for saving progress." << std::endl;
        }
    }

    void load_progress(const std::string &filename) override {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string temp_name;
            std::getline(file, temp_name);
            player.set_name(temp_name);
            int temp_score;
            file >> temp_score;
            player.set_score(temp_score);
            file.ignore();  // Ignore the newline character after the score

            std::getline(file, word_to_guess);
            std::getline(file, guessed_word);
            file >> max_attempts;
            file >> current_attempts;

            guessed_letters.clear();
            char letter;
            while (file >> letter) {
                guessed_letters.insert(letter);
            }
            file.close();
        } else {
            std::cerr << "Unable to open file for loading progress." << std::endl;
        }
    }
};

int main() {
    std::string player_name;
    std::cout << "Enter player name: ";
    std::cin >> player_name;

    HangmanPlayer player(player_name);

    // List of words to choose from
    std::vector<std::string> words = {"example", "hangman", "programming", "openai", "cplusplus"};

    Hangman game(player, words, 6);

    game.start();
    game.end();

    return 0;
}
