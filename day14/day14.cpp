#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <stdlib.h>
#include <algorithm>

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

unsigned long apply_mask(unsigned long value, string mask) {
    for (int i{0}; i < mask.length(); ++i) {
        if (mask[i] == '0') {
            value &= ~(1UL << i);
        } else if (mask[i] == '1') {
            value |= 1UL << i;
        }
    }

    return value;
}

int main() {
    ifstream file ("input.txt");
    string input{};

    map<unsigned long, unsigned long> mem{};
    string mask;

    if (file.is_open()) {
        while(getline(file, input)){
            vector<string> instruction{split(input, " ")};
            if (instruction[0].find("mask") != string::npos) {

                mask = instruction[2];
                reverse(mask.begin(), mask.end());
            } else {
                auto adressStart = instruction[0].find("[") + 1;
                auto adressEnd = instruction[0].find("]");
                unsigned long adress = stoul(instruction[0].substr(adressStart, adressEnd - adressStart));
                unsigned long raw = stoul(instruction[2]);

                mem[adress] = apply_mask(raw, mask);
            }
        }
        file.close();

        unsigned long sum{0U};
        for (auto val : mem) {
            sum += val.second;
        }

        cout << "Sum is " << sum << endl;
    }
}