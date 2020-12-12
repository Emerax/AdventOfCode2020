#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <cmath>

using namespace std;

enum DIR{
    EAST, SOUTH, WEST, NORTH
};

int main() {
    ifstream file ("input.txt");
    string input{};

    int x{0}; 
    int y{0};
    DIR heading{EAST};

    int x_way{10};
    int y_way{1};
    long x_ship{0};
    long y_ship{0};

    if (file.is_open()) {
        while(getline(file, input)){
            string move = input.substr(0, 1);
            int amount = stoi(input.substr(1));
            if (move == "N"){
                y += amount;

                y_way += amount;
            } else if (move == "S") {
                y -= amount;

                y_way -= amount;
            } else if (move == "E") {
                x += amount;

                x_way += amount;
            } else if (move == "W") {
                x -= amount;

                x_way -= amount;
            } else if (move == "L") {
                int turns{amount / 90};
                for (int i{0}; i < turns; ++i) {
                    switch (heading)
                    {
                    case EAST:
                        heading = NORTH;
                        break;
                    case SOUTH:
                        heading = EAST;
                        break;
                    case WEST:
                        heading = SOUTH;
                        break;
                    case NORTH:
                        heading = WEST;
                        break;
                    }

                    int temp{x_way};
                    x_way = -y_way;
                    y_way = temp;
                }
            } else if (move == "R") {
                int turns{amount / 90};
                for (int i{0}; i < turns; ++i) {
                    switch (heading)
                    {
                    case EAST:
                        heading = SOUTH;
                        break;
                    case SOUTH:
                        heading = WEST;
                        break;
                    case WEST:
                        heading = NORTH;
                        break;
                    case NORTH:
                        heading = EAST;
                        break;
                    }

                int temp{x_way};
                x_way = y_way;
                y_way = -temp;
                }
            } else if (move == "F") {
                switch (heading)
                {
                case NORTH:
                    y += amount;
                    break;
                case SOUTH:
                    y -= amount;
                    break;
                case EAST:
                    x += amount;
                    break;
                case WEST:
                    x -= amount;
                    break;
                }

                x_ship += amount * x_way;
                y_ship += amount * y_way;

            }

        }
        file.close();

        int distance = abs(x) + abs(y);
        cout << "Ferry arrives at (" << x << ", " << y << ") having moved a distance of " << distance << endl;

        long way_distance{abs(x_ship) + abs(y_ship)};
        cout << "Using correct movement rules, ferry arrives at (" << x_ship << ", " << y_ship << ") having moved a distance of " << way_distance << endl;
    }
}