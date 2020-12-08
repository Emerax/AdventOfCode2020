#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <stdlib.h>

using namespace std;

struct op {
    string in{};
    int op{};
};

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

vector<op> parse_program(ifstream file) {
    string input;
    vector<op> program{};
        

    return program;
}

int main() {
    ifstream file ("input.txt");
    string input{};

    vector<op> program{};
    set<unsigned int> seen{};

    if (file.is_open()) {
        while(getline(file, input)){
            vector<string> words = split(input, " ");
            program.push_back(op{words[0], stoi(words[1])});
        }
        file.close();

        unsigned int accumulator{0};

        unsigned int next_op{0};

        int fix_attempt{0};
        int op_count{0};
        bool found_fix{false};

        while(!found_fix) {
            if (next_op >= program.size()) {
                cout << "Program terminated successfully, accumulator: " << accumulator << ". Had to attempt " << fix_attempt << " fixes." << endl;
                found_fix = true;
            } else if (seen.find(next_op) != seen.end()) {
                cout << "Program looped back, accumulator: " << accumulator << endl;
                ++fix_attempt;
                op_count = 0;
                seen.clear();
                next_op = 0;
                accumulator = 0;
                continue;
            } else {
                //Next op adujsted by one to match file lines
                cout << "Running op: " << next_op + 1 << endl;
                seen.insert(next_op);
                op current{program[next_op]};
                if (current.in == "acc") {
                    accumulator += current.op;
                    ++next_op;
                } else if (current.in == "jmp") {
                    if (op_count == fix_attempt) {
                        ++next_op;
                    } else {
                        next_op += current.op;
                    }
                    ++op_count;
                } else if (current.in == "nop") {
                    if (op_count == fix_attempt) {
                        next_op += current.op;
                    } else {
                        ++next_op;
                    }
                    ++op_count;
                }
            }
        }
    }
}