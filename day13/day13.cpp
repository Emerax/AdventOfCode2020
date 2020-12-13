#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <limits.h>
#include <map>
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

int main() {
    ifstream file ("input.txt");
    string input{};

    if (file.is_open()) {
        getline(file, input);
        int earliest{stoi(input)};
        getline(file, input);
        vector<string> ids = split(input, ",");
        file.close();

        pair<int, int> best{-1, INT_MAX};

        for(auto id : ids) {
            if (id != "x") {
                int roundtrip{stoi(id)};
                int next_departure{roundtrip * (earliest / roundtrip) + roundtrip};

                if (next_departure < best.second) {
                    best.first = roundtrip;
                    best.second = next_departure;
                }
            }
        }
        
        cout << "Best buss found has id: " << best.first << ", it departs at: " << best.second << ". Answer is: " << best.first * (best.second - earliest) << endl;

        map<int, unsigned long> vals{};

        for(int i{0}; i < ids.size(); ++i) {
            if (ids[i] != "x") {
                vals.insert({i, stoi(ids[i])});
            }
        }

        unsigned long t{0};

        while(true) {

            unsigned long step{1};
            bool found_it{true};
            for(auto val: vals) {
                if ((t + val.first) % val.second != 0) {
                    found_it = false;
                    break;
                }


                //Found a matching bus!
                step *= val.second;
            }

            if (found_it) {
                break;
            }

            t += step;
        }


        cout << "Found the number! Number is: " << t << endl;
    }
}