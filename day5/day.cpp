#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>

using namespace std;

struct seat{
    int row{0};
    int column{0};
    int id{0};
};

int seat_id(int row, int col) {
    return row * 8 + col;
}

seat decode_seat(string code) {
    int row_min{0}, row_max{128};
    int col_min{0}, col_max{8};
    for (auto c : code) {
        if (c == 'F'){
            row_max -= (row_max - row_min) / 2;
        } else if (c == 'B') {
            row_min += (row_max - row_min) / 2;
        } else if (c == 'L') {
            col_max -= (col_max - col_min) / 2;
        } else if (c == 'R') {
            col_min += (col_max - col_min) / 2;
        }
    }
    
    return seat{row_min, col_min, seat_id(row_min, col_min)};
}

int main() {
    string input;
    ifstream file ("input.txt");
    list<int> seat_ids{};

    int highest_id{0};

    if (file.is_open()) {
        while(getline(file, input)){
            seat s = decode_seat(input);
            seat_ids.push_back(s.id);
            if (s.id > highest_id) {
                highest_id = s.id;
            }
        }
        file.close();
    }

    seat_ids.sort();
    int missing{0};

    for(auto it{seat_ids.begin()}; it != seat_ids.end(); ++it) {
        int id = *it;
        if (*next(it) == id + 2) {
            missing = id + 1;
        }
    }

    cout << "Highest id seen was: " << highest_id << endl;
    cout << "Missing seat found: " << missing << "!" << endl;
}