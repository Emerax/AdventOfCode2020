#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <stdlib.h>
#include <algorithm>

using namespace std;

struct bag {
    vector<pair<int, string>> contents{};
    int contain_count{-1};
};

bool debug{false};

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

int bag_count(string color, map<string, bag> bags) {
    if (bags[color].contain_count != -1) {
        return bags[color].contain_count;
    } else {
        int total{0};
        for (auto p : bags[color].contents) {
        total += p.first * (1 + bag_count(p.second, bags));
        }

        bags[color].contain_count = total;
        return total;
    }
}

bool carries_shiny_gold(string color, map<string, bag> bags) {
    bool found_it = any_of(bags[color].contents.begin(), bags[color].contents.end(), [](pair<int, string> p){return p.second == "shiny gold";} );
    if (found_it) {
        return true;
    } else {
        bool indirect_carrier{false};
        for (auto it{bags[color].contents.begin()}; it != bags[color].contents.end(); ++it) {
            if(carries_shiny_gold(it->second, bags)) {
                return true;
            }
        }
        return false;
    }
}

int main() {
    string input;
    ifstream file ("input.txt");

    map<string, bag> bags{};

    if (file.is_open()) {
        while(getline(file, input)){
            vector<string> rule = split(input, " ");
            string bag_color = rule[0] + " " + rule[1];
            if (rule[4] == "no") {
                bags.insert(pair<string, bag>(bag_color, bag{}));
            } else {
                vector<pair<int, string> > containage{};
                for (int i{4}; i < rule.size(); i += 4) {
                    int number{stoi(rule[i])};
                    string color = rule[i + 1] + " " + rule[i + 2];
                    containage.push_back(pair<int, string>(number, color));
                }
                bag new_bag{containage};
                bags.insert({bag_color, new_bag});
            }
        }
        file.close();

        int total_shiny_gold_carriers{0};
        for (auto it{bags.begin()}; it != bags.end(); ++it) {
            if (carries_shiny_gold(it->first, bags)) {
                ++total_shiny_gold_carriers;
            }
            debug = false;
        }

        cout << "Found " << total_shiny_gold_carriers << " bags capable of carrying shiny gold bags!" << endl;
        cout << "A single shiny gold bag must contain " << bag_count("shiny gold", bags) << " nested bags" << endl;
    }
}