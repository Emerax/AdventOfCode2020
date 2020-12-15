#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stdlib.h>

using namespace std;

vector<string> split(string s, string delimiter) {
    size_t start{0U}, end{s.find(delimiter)};
    vector<string> result{};

    while(end != string::npos) {
        result.push_back(s.substr(start, end - start));

        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }

    result.push_back(s.substr(start, end - start));

    return result;
}

int main() {
    ifstream file ("input.txt");
    string input{};

    if (file.is_open()) {
        getline(file, input);
        vector<string> starting_numbers = split(input, ",");
        file.close();

        //<number, turn occurence> map.
        map<unsigned, unsigned> seen{};
        unsigned turn{1};

        for (auto num : starting_numbers) {
            seen[stoi(num)] = turn;
            ++turn;
        }

        unsigned next{0};

        while(turn < 30000000) {
            auto it{seen.find(next)};
            if (it == seen.end()) {
                //New number
                seen[next] = turn;
                next = 0;
            } else {
                unsigned age{turn - it->second};
                seen[next] = turn;
                next = age;
            }

            ++turn;
            if (turn == 2020) {
                cout << "Turn " << turn << " results in number " << next << endl;
            }
        }

        cout << "Turn " << turn << " results in number " << next << endl;
    }
}