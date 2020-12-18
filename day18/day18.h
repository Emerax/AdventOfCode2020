#include <vector>

using namespace std;

enum OPERATOR{
    ADD, SUB, MULT, DONE
};

unsigned long math(unsigned long left, OPERATOR op, vector<string> expression);