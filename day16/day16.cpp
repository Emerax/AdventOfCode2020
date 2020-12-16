#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include "day16.h"
#include <algorithm>
#include <sstream>

using namespace std;

bool follows_rule(int value, rule rule) {
    return (rule.lower_low <= value && value <= rule.lower_high)
    || (rule.higher_low <= value && value <= rule.higher_high);
}

bool possible_valid_ticket(vector<int> ticket, vector<rule> rules) {
    for (auto rule: rules) {
        for (auto value : ticket) {
            if (!follows_rule(value, rule)) {
                return false;
            }
        }
    }

    return true;
}

rule parse_rule(string s) {
    rule r{};
    auto colon_index = s.find(":");

    r.name = s.substr(0, colon_index);
    vector<string> fields = split(s.substr(colon_index + 2), " ");

    string range{fields[0]};
    auto hyphen_pointer{range.find("-")};
    r.lower_low = stoi(range.substr(0, hyphen_pointer));
    r.lower_high = stoi(range.substr(hyphen_pointer + 1));

    range = fields[2];
    hyphen_pointer = range.find("-");
    r.higher_low = stoi(range.substr(0, hyphen_pointer));
    r.higher_high = stoi(range.substr(hyphen_pointer + 1));

    return r;
}

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

string rule_to_string(rule r) {
    stringstream oss;
    oss << r.lower_low << "-" << r.lower_high << ", " << r.higher_low << "-" << r.higher_high;
    return oss.str();
}

int main() {
    ifstream file ("input.txt");
    string input{};

    vector<rule> rules{};
    vector<int> own_ticket{};
    vector<vector<int> > tickets{};

    if (file.is_open()) {
        while(getline(file, input)) {
            //Parse rules
            if (input == "") {
                break;
            } else {
                rule r{parse_rule(input)};
                rules.push_back(r);
            }
        }

        //throw away title
        getline(file, input);
        while(getline(file, input)) {
            //Parse own ticket
            if (input == "") {
                break;
            } else {
                vector<string> vals{split(input, ",")};
                for (auto val: vals) {
                    own_ticket.push_back(stoi(val));
                }
            }
        }

        //throw away title
        getline(file, input);
        while(getline(file, input)) {
            //Parse own remaining tickets
            if (input == "") {
                break;
            } else {
                vector<int> ticket{};
                for (auto val: split(input, ",")) {
                    ticket.push_back(stoi(val));
                }
                tickets.push_back(ticket);
            }
        }
        file.close();

        vector<int> invalid{};
        vector<vector<int>> valid_tickets{};

        for (auto ticket : tickets) {
            bool valid_ticket{true};
            for (auto value : ticket) {
                bool valid{false};
                for (auto rule : rules) {
                    if (follows_rule(value, rule)) {
                        valid = true;
                        break;
                    }
                }

                if(!valid) {
                    invalid.push_back(value);
                    valid_ticket = false;
                }
            }

            if (valid_ticket) {
                valid_tickets.push_back(ticket);
            }
        }

        valid_tickets.push_back(own_ticket);

        int error_rate{0};
        for (auto value : invalid) {
            error_rate += value;
        }

        cout << "Ticket scanning error rate: " << error_rate << endl;

        vector<rule*> unassigned_rules{};
        for (int i{0}; i < rules.size(); ++i) {
            unassigned_rules.push_back(&rules[i]);
        }

        string garbage{};

        while(unassigned_rules.size() > 0) {
            for (int i{0}; i < valid_tickets[0].size(); ++i) {
                vector<int> possible_rules{};
                for (int rule_index{0}; rule_index < unassigned_rules.size(); ++rule_index) {
                    if (all_of(valid_tickets.begin(), valid_tickets.end(), [i, unassigned_rules, rule_index](auto t) {
                        return follows_rule(t[i], *(unassigned_rules[rule_index]));
                    })) {
                        possible_rules.push_back(rule_index);
                    }

                    if (possible_rules.size() > 1) {
                        break;
                    }
                }

                if(possible_rules.size() == 1) {
                    unassigned_rules[possible_rules[0]]->index = i;
                    unassigned_rules.erase(unassigned_rules.begin() + possible_rules[0]);
                }
            }
        }

        unsigned long answer{1};

        for (auto rule : rules) {
            //cout << "Rule " << rule.name << " has index " << rule.index << ", ticket value is: " << own_ticket[rule.index] << endl;
            if (rule.name.find("departure") != string::npos) {
                cout << "Rule " << rule.name << endl;
                answer *= own_ticket[rule.index];
            }
        }

        cout << "Answer is " << answer << endl;
    }
}