#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <regex>
#include <boost/regex.hpp>
#include <pcrecpp.h>

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

string string_rule(vector<string> rule) {
    string s;
    for (const auto &piece : rule) s += piece;
    return s;
}

map<string, vector<string>> collapse_grammar(map<string, vector<string>> grammar) {
    if (grammar.size() == 1) {
        return grammar;
    }

    map<string, string> leaf_rules{};
    //Find the leaf rules.
    for (auto rule : grammar) {
        if (rule.second.size() == 1) {
            leaf_rules.insert({rule.first, rule.second[0]});
        }
    }

    //Erase them from the grammar.
    for (auto leaf : leaf_rules) {
        grammar.erase(leaf.first);
    }

    //Apply leaf rules.
    for (auto rule : grammar) {
        for (auto leaf : leaf_rules) {
            for (int i{0}; i < grammar[rule.first].size(); ++i) {
                if (grammar[rule.first][i] == leaf.first) {
                    grammar[rule.first][i] = leaf.second;
                }
            }
        }
    }

    //Check if any entries are only literals and possibly [|()]-symbols.
    //Replace rules with their literal counterparts.
    for (auto rule : grammar) {
        bool is_leaf{true};
        for (auto w : rule.second) {
            if (w.find("\"") == string::npos && w != "|" && w != "(" && w != ")") {
                is_leaf = false;
                break;
            }
        }

        if (is_leaf) {
            string literal{};
            for (string w : rule.second) {
                w.erase(std::remove(w.begin(), w.end(), '\"'), w.end());
                literal += w;
            }

            literal = "\"" + literal + "\"";
            grammar[rule.first] = vector<string>{literal};
        }
    }

    return collapse_grammar(grammar);
}

int main() {
    string input;
    fstream file("input.txt");

    if(file.is_open()) {
        map<string, vector<string>> grammar{};
        map<string, vector<string>> loop_grammar{};

        getline(file, input);
        while(input != "") {
            vector<string> raw = split(input, " ");
            string symbol{raw.front().substr(0, raw.front().size() - 1)};

            raw.erase(raw.begin(), raw.begin() + 1);
            auto or_it{find(raw.begin(), raw.end(), "|")};
            if (or_it != raw.end()) {
                //Rule is of or-form. Add parenhteses.
                raw.insert(raw.begin(), "(");
                raw.insert(raw.end(), ")");
            }

            grammar.insert({symbol, raw});

            //For part 2, replace rules with handwritten regexes, allowing for the grammar collapser
            //to still work on the grammar symbols.
            if (symbol == "8") {
                loop_grammar.insert({symbol, {"(", "42", ")", "\"+\""}});
            } else if (symbol == "11") {
                loop_grammar.insert({symbol, {"\"(?\'X\'\"", "42", "\"(?&X)?\"", "31", ")"}});
            } else {
                loop_grammar.insert({symbol, raw});
            }

            getline(file, input);
        }
        grammar = collapse_grammar(grammar);
        loop_grammar = collapse_grammar(loop_grammar);
        //Strip excess "'s from final rule, and it can be used as a regex string.
        grammar["0"][0] = grammar["0"][0].substr(1, grammar["0"][0].size() - 2);
        loop_grammar["0"][0] = loop_grammar["0"][0].substr(1, loop_grammar["0"][0].size() - 2);
        //Add start-of-string and end-of-string anchors to the regex.
        loop_grammar["0"][0] = "^" + loop_grammar["0"][0] + "$";

        regex r{grammar["0"][0]};
        string loop_regex_string{loop_grammar["0"][0]};
        pcrecpp::RE loop_r{loop_regex_string};
        if (!loop_r.error().empty()) {
            cout << "PCRE error: " << loop_r.error() << endl;
        }

        int matches{0};
        int loop_matches{0};

        //Read the lines to check against the rules.
        while(getline(file, input)) {
            if (regex_match(input, r)) {
                ++matches;
            }

            if (loop_r.FullMatch(input)) {
                ++loop_matches;
            }
        }
        file.close();

        cout << matches << " strings match the rules" << endl;
        cout << loop_matches << " strings match the looping rules" << endl;
    }
}