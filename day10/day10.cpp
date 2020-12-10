#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

map<int, unsigned long> cache{};

unsigned long count_arrangements(int start, vector<int> adapters) {
    if (cache.find(start) != cache.end()) {
        return cache[start];
    }

    unsigned long total{0};
    int base{0};
    if (start > 0) {
        base = adapters[start - 1];
    }
    for (int i{start}; i < adapters.size() && i <= start + 3; ++i) {
        if (adapters[i] - base < 4) {
            total += count_arrangements(i + 1, adapters);
        }
    }
    //Check if we can connect to the device. Every iteration that gets here is a legal configuration.
    if ((adapters.back() + 3) - base < 4) {
        ++total;
    }

    cache.insert(pair<int, unsigned long>{start, total});

    return total;
}

int main() {
    ifstream file ("input.txt");
    string input{};

    vector<int> difference_count{0, 0, 0};
    vector<int> adapters{};

    if (file.is_open()) {
        while(getline(file, input)){
            adapters.push_back(stoi(input));
        }
        file.close();

        sort(adapters.begin(), adapters.end());

        for (int i{0}; i < adapters.size(); ++i) {
            if (i == 0) {
                ++difference_count[adapters[0] - 1];
            } else {
                int difference{adapters[i] - adapters[i - 1]};
                ++difference_count[difference - 1];
            }
        }

        cout << "Found " << difference_count[0] << " 1-diffs and " << difference_count[2] + 1 << " 3-diffs, product: " << difference_count[0] * (difference_count[2] + 1)  << endl;
        
        unsigned long arrangements{count_arrangements(0, adapters)};

        // //Check number of adapters that fit into wall socket.
        // for (int i{0}; i < 3; ++i) {
        //     int multiplier{0};
        //     if (adapters[i] < 4) {
        //     }
        //     arrangements = multiplier;
        // }

        // for (int i{0}; i < adapters.size(); ++i) {
        //     int base{adapters[i]};
        //     unsigned int multiplier{0};
        //     for (int j{1}; j < 4; ++j) {
        //         if (i + j < adapters.size()) {
        //             if (adapters[i + j] - base < 4) {
        //                 ++multiplier;
        //             }
        //         }
        //     }

        //     //Check if we can connect to device as well.
        //     if ((adapters.back() + 3) - base < 4) {
        //         ++multiplier;
        //     }

        //     cout << "Base: " << base << ", following adapters: " << adapters[i + 1] << ", " << adapters[i + 2] << ", " << adapters[i + 3] << ", " << (adapters.back() + 3) << " and multiplier thus: " << multiplier << endl;

        //     if (multiplier > 0) {
        //         arrangements *= multiplier;
        //     }
        // }

        cout << "Calculated " << arrangements << " different arrangements of adapters" << endl;
    }
}