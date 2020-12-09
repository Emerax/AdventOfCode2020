#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <stdlib.h>
#include <deque>
#include <limits.h>

using namespace std;

set<unsigned long> generate_sums(deque<unsigned long> deq) {
    set<unsigned long> sums{};
    for (int i{0}; i < deq.size(); ++i) {
        for (int j{i + 1}; j < deq.size(); ++j) {
            sums.insert(deq[i] + deq[j]);
        }
    }

    return sums;
}

pair<unsigned long, unsigned long> find_smallest_and_largest(vector<unsigned long> list) {
    unsigned long smallest{0U}; 
    unsigned long largest{ULONG_MAX};

    for (auto v : list) {
        if (v > smallest) {
            smallest = v;
        }

        if (v < largest) {
            largest = v;
        }
    }

    return pair<unsigned long, unsigned long>{smallest, largest};
}

int main() {
    ifstream file ("input.txt");
    string input{};

    deque<unsigned long> preamble{};
    vector<unsigned long> values{};

    unsigned long invalid{0};

    int preamble_count{25};
    int counter{1};

    if (file.is_open()) {
        while(getline(file, input)){
            values.push_back(stoul(input));
            if (counter <= preamble_count) {
                preamble.push_front(stoul(input));
                ++counter;
            } else {
                set<unsigned long> sums = generate_sums(preamble);
                unsigned long value = stoul(input);
                if(sums.find(value) == sums.end()) {
                    invalid = value;
                    cout << "Found invalid number: " << value << endl;
                }

                preamble.pop_back();
                preamble.push_front(value);
            }
        }
        file.close();

        int start{0};
        int cursor{0};
        vector<unsigned long> terms{};

        unsigned long sum{0};

        while(start < values.size()) {
            sum += values[cursor];
            terms.push_back(values[cursor]);
            if (sum == invalid) {
                pair<unsigned long, unsigned long> weakness_terms = find_smallest_and_largest(terms);
                cout << "Found the sequence, weakness is: " << weakness_terms.first + weakness_terms.second << endl;
                return 1;
            } else if (sum > invalid) {
                ++start;
                cursor = start;
                terms.clear();
                sum = 0;
            } else {
                ++cursor;
            }
        }
    }
}