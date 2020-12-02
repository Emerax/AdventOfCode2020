#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>

using namespace std;

int main() {
    string input;
    ifstream file ("input.txt");

    int correct_passwords{0};

    int new_correct_passwords{0};

    if (file.is_open()) {
        while(getline(file, input)){

            int min = stoi(input.substr(0, input.find('-')));
            int max = stoi(input.substr(input.find('-') + 1, input.find(' ')));

            char symbol = *input.substr(input.find(' ') + 1, 1).c_str();

            string password = input.substr(input.find(": "));

            int occurence{count(password.begin(), password.end(), symbol)};

            if(occurence >= min && occurence <= max) {
                ++correct_passwords;
            }

            if(password[min + 1] == symbol ^ password[max + 1] == symbol) {
                ++new_correct_passwords;
            }
        }
        file.close();
    }

    cout << "Found " << correct_passwords << " correct passwords!" << endl;

    cout << "Found " << new_correct_passwords << " correct passwords using the correct policy!" << endl;
}