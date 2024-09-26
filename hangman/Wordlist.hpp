/* 
this class loads in the words in the words.txt into a vector
the getRandomWord fetches a random word from that vector when called
*/

#include <cstdlib> // 
#include <ctime> // for time function
#include <fstream> // for files
#include <vector>
#include <iostream>

#ifndef WORDLIST_HPP
#define WORDLIST_HPP
using namespace std;
class WordList {
private:
    vector<string> words; // a vector to store our words
public:
    WordList(const string& filename) {
        // opens a file called filename
        fstream file(filename); 
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            exit(1);
        }

        // puts every words in the file into vector
        string word;
        while (file >> word) {
            words.push_back(word);
        }
        file.close();
    }
    string getRandomWord() {
        
        // gets random word from the vector of words
        srand(static_cast<unsigned int>(time(0)));
        return words[rand() % words.size()];
    }
};

#endif // WORDLIST_H
