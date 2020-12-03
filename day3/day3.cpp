#include <iostream>
#include <fstream>
#include <vector>
#include "day3.h"

using namespace std;

int main() {
    string input;
    ifstream file ("input.txt");

    vector<string> slope;

    if (file.is_open()) {
        while(getline(file, input)){
            slope.push_back(input);
        }
        file.close();
    }

    unsigned int initialSlope{find_trees(slope, 3, 1)};

    unsigned int answer{initialSlope};

    answer *= find_trees(slope, 1, 1);
    answer *= find_trees(slope, 5, 1);
    answer *= find_trees(slope, 7, 1);
    answer *= find_trees(slope, 1, 2);

    cout << "Slope (3, 1) hits " << initialSlope << " trees!" << endl;
    cout << "Product of all slopes is " << answer << "!" << endl;
}

int find_trees(vector<string> slope, int deltaX, int deltaY) {
    long unsigned int width{slope[0].size()}, height{slope.size()};
    int trees{0};

    int x{0}, y{0};

    do {
        if (slope[y][x] == '#') {
            ++trees;
        }

        y += deltaY;
        x = (x + deltaX < width) ? x + deltaX : (x + deltaX) % width;

    } while (y < height);

    return trees;
}