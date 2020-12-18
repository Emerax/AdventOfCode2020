#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include "day18.h"

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

ptrdiff_t find_sub_expression_end(vector<string> expression) {
    ptrdiff_t count{0};
    for (int i{0}; i < expression.size(); ++i) {
        count += std::count(expression[i].begin(), expression[i].end(), '(');
        count -= std::count(expression[i].begin(), expression[i].end(), ')');

        if (count == 0) {
            return i;
        }
    }
}

ptrdiff_t find_sub_expression_end(vector<string> expression, int start_index) {
    ptrdiff_t count{0};
    for (int i{start_index}; i < expression.size(); ++i) {
        count += std::count(expression[i].begin(), expression[i].end(), '(');
        count -= std::count(expression[i].begin(), expression[i].end(), ')');

        if (count == 0) {
            return i;
        }
    }
}

string stringify_op(OPERATOR op) {
    switch (op)
    {
    case ADD:
        return "+";
        break;
    case SUB:
        return "-";
        break;
    case MULT:
        return "*";
        break;
    default:
        return "DONE";
        break;
    }
}

OPERATOR parse_op(string s) {
    if (s == "+") {
        return ADD;
    } else if (s == "-") {
        return SUB;
    } else if (s == "*") {
        return MULT;
    }
}

size_t first_parenthesis_index(vector<string> expression) {
    for (size_t i{0}; i < expression.size(); ++i) {
        auto searcher{expression[i].find("(")};
        if (searcher != string::npos) {
            return i;
        }
    }

    return string::npos;
}

unsigned long advanced_math(vector<string> expression) {
    //Parse all parentheses.
    size_t start_index{first_parenthesis_index(expression)};
    while(start_index != string::npos) {
        //Find parenthesis, calculate value, replace all elements with value.
        auto end_index = find_sub_expression_end(expression, start_index);
        vector<string> sub_expression{expression.begin() + start_index, expression.begin() + end_index + 1};
        //Prune away the parentheses.
        sub_expression[0] = sub_expression[0].substr(1);
        sub_expression.back() = sub_expression.back().substr(0, sub_expression.back().size() - 1);
        auto resolution{advanced_math(sub_expression)};
        //Replace parenthesis elements with the new value;
        expression.erase(expression.begin() + start_index, expression.begin() + end_index + 1);
        expression.insert(expression.begin() + start_index, to_string(resolution));
        //Check if any parentheses remain.
        start_index = first_parenthesis_index(expression);
    }
    
    //Resolve all additions
    auto plus_it{find(expression.begin(), expression.end(), "+")};
    while (plus_it != expression.end()) {
        unsigned long sum{stoul(*(plus_it - 1)) + stoul(*(plus_it + 1))};
        //Replace sum elements with result
        expression.erase(plus_it - 1, plus_it + 2);
        expression.insert(plus_it - 1, to_string(sum));
        //Check for remaining sums
        plus_it = find(expression.begin(), expression.end(), "+"); 
    }

    //Only multiplications should remain, calculate with "normal" math
    return math(1, MULT, expression);
}

unsigned long math(unsigned long left, OPERATOR op, vector<string> expression) {
    if (expression.size() == 0) {
        return left;
    }

    unsigned long value{0};
    vector<string> rest_of_expression{};
    OPERATOR next_op{};
    if (expression[0][0] == '(') {
        //Next value is a parenthesis
        ptrdiff_t sub_expression_end{find_sub_expression_end(expression)};
        vector<string> sub_expression{expression.begin(), expression.begin() + sub_expression_end + 1};
        //Prune away the enclosing parentheses.
        sub_expression[0] = sub_expression[0].substr(1);
        sub_expression.back() = sub_expression.back().substr(0, sub_expression.back().size() - 1);
        value = math(0, ADD, sub_expression);
        if (sub_expression_end == expression.size() - 1) {
            next_op = DONE;
        } else {
            next_op = parse_op(expression[sub_expression_end + 1]);
            rest_of_expression = vector<string>{expression.begin() + sub_expression_end + 2, expression.end()};
        }
    } else {
        //Next value is a number.
        value = stoul(expression[0]);
        if (expression.size() > 1) {
            next_op = parse_op(expression[1]);
            rest_of_expression = vector<string>{expression.begin() + 2, expression.end()};
        } else {
            next_op = DONE;
        }
    }

    switch (op) {
    case ADD:
        return math(left + value, next_op, rest_of_expression);
        break;
    case SUB:
        return math(left - value, next_op, rest_of_expression);
        break;
    case MULT:
        return math(left * value, next_op, rest_of_expression);
        break;
    
    default:
        cout << "This should never happen" << endl;
        break;
    }

}

int main() {
    string input;
    fstream file("input.txt");

    unsigned long long sum{0};
    unsigned long advanced_sum{0};

    while(getline(file, input)) {
        vector<string> expression{split(input, " ")};
        sum += math(0, ADD, expression);

        vector<string> advanced_expression{split(input, " ")};
        advanced_sum += advanced_math(advanced_expression);
    }

    file.close();

    cout << "Sum is " << sum << endl;

    cout << "Meanwhile, the advanced sum is: " << advanced_sum << endl;
}