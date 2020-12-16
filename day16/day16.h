#include <iostream>
#include <vector>

using namespace std;

struct rule {
    string name;
    int lower_low;
    int lower_high;
    int higher_low;
    int higher_high;
    int index{-1};
};

vector<string> split(string s, string delimiter);
string rule_to_string(rule r);