#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <bitset>

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

unsigned long apply_mask_v1(unsigned long value, string mask) {
    for (int i{0}; i < mask.length(); ++i) {
        if (mask[i] == '0') {
            value &= ~(1UL << i);
        } else if (mask[i] == '1') {
            value |= 1UL << i;
        }
    }

    return value;
}

string apply_mask_v2(string s, string mask) {
    for (int i{0}; i < mask.length(); ++i) {
        if (mask[i] == '1') {
            s.replace(i, 1, "1");
        } else if (mask[i] == 'X') {
            s.replace(i, 1, "X");
        }
    }

    return s;
}

void collapse_wave_function(string quantum_adress, vector<unsigned long> &adresses) {
    auto x_pos = quantum_adress.find("X");
    if (x_pos == string::npos) {
        reverse(quantum_adress.begin(), quantum_adress.end());
        adresses.push_back(bitset<64>(quantum_adress).to_ullong());
    } else {
        collapse_wave_function(quantum_adress.replace(x_pos, 1, "0"), adresses);
        collapse_wave_function(quantum_adress.replace(x_pos, 1, "1"), adresses);
    }
} 

int main() {
    ifstream file ("input.txt");
    string input{};

    map<unsigned long, unsigned long> mem1{};
    map<unsigned long, unsigned long> mem2{};
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

                mem1[adress] = apply_mask_v1(raw, mask);
                
                string quantum_adress = bitset<64>(adress).to_string();
                reverse(quantum_adress.begin(), quantum_adress.end());
                quantum_adress = apply_mask_v2(quantum_adress, mask);
                vector<unsigned long> adresses{};
                collapse_wave_function(quantum_adress, adresses);



                for (auto adress: adresses) {
                    mem2[adress] = raw;
                }
            }
        }
        file.close();

        unsigned long sum_v1{0U};
        for (auto val : mem1) {
            sum_v1 += val.second;
        }

        unsigned long sum_v2{0U};
        for (auto val: mem2) {
            sum_v2 += val.second;
        }

        cout << "Version 1 sum is " << sum_v1 << endl;
        cout << "Version 2 sum is " << sum_v2 << endl;
    }
}