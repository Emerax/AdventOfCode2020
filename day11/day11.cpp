#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

void print_grid(vector<string> grid) {
    for(auto l : grid) {
        cout << l << endl;
    }
    cout << endl;
}

int count_visible(int x, int y, vector<string> grid) {
    vector<int> diff{-1, 0, 1};

    int visible{0};

    for (auto dx : diff) {
        for (auto dy : diff) {
            if (dx != 0 || dy != 0) {
                int x_pos{x + dx};
                int y_pos{y + dy};
                while(x_pos < grid.size() && y_pos < grid[x_pos].length()) {
                    if (grid[x_pos][y_pos] == '#') {
                        ++visible;
                        break;
                    } else if (grid[x_pos][y_pos] == 'L') {
                        break;
                    }
                    x_pos += dx;
                    y_pos += dy;
                }
            }
        }
    }

    return visible;
}

int count_adjacent(int x, int y, vector<string> grid) {
    vector<int> diff{-1, 0, 1};

    int adjacent{0};

    for (auto dx : diff) {
        for (auto dy : diff) {
            if (dx != 0 || dy != 0) {
                int x_pos{x + dx};
                int y_pos{y + dy};

                if (x_pos < grid.size() && y_pos < grid[x_pos].length()) {
                    if (grid[x_pos][y_pos] == '#') {
                        ++adjacent;
                    }
                }
            }
        }
    }

    return adjacent;
}

vector<string> iterate(vector<string> grid, bool &changed) {
    vector<string> next_state{};
    for (int x{0}; x < grid.size(); ++x) {
        string line{grid[x]};
        string next_line{""};
        for (int y{0}; y < line.length(); ++y) {
            auto symbol{line[y]};
            if (symbol == 'L') {
                if (count_adjacent(x, y, grid) == 0) {
                    next_line.append("#");
                    changed = true;
                } else {
                    next_line.append("L");
                }
            } else if (symbol == '#') {
                if (count_adjacent(x, y, grid) >= 4) {
                    next_line.append("L");
                    changed = true;
                } else {
                    next_line.append("#");
                }
            } else {
                next_line.append(string{symbol});
            }
        }

        next_state.push_back(next_line);
    }

    return next_state;
}

vector<string> iterate_visible(vector<string> grid, bool &changed) {
    vector<string> next_state{};
    for (int x{0}; x < grid.size(); ++x) {
        string line{grid[x]};
        string next_line{""};
        for (int y{0}; y < line.length(); ++y) {
            auto symbol{line[y]};
            if (symbol == 'L') {
                if (count_visible(x, y, grid) == 0) {
                    next_line.append("#");
                    changed = true;
                } else {
                    next_line.append("L");
                }
            } else if (symbol == '#') {
                if (count_visible(x, y, grid) >= 5) {
                    next_line.append("L");
                    changed = true;
                } else {
                    next_line.append("#");
                }
            } else {
                next_line.append(string{symbol});
            }
        }

        next_state.push_back(next_line);
    }

    return next_state;
}

int main() {
    ifstream file ("input.txt");
    string input{};

    vector<string> grid{};

    if (file.is_open()) {
        while(getline(file, input)){
            grid.push_back(input);
        }
        file.close();
    }

    vector<string> grid_visible{grid};

    while(true) {
        bool state_changed{false};
        vector<string> next_state{iterate(grid, state_changed)};

        if(state_changed) {
            grid = next_state;
        } else {
            break;
        }
    }

    int occupied_seats{0};
    for_each(grid.begin(), grid.end(), [&occupied_seats](string s){
        occupied_seats += count(s.begin(), s.end(), '#');
    });

    cout << "Found the steady state, seats occupied: " << occupied_seats << endl;

    while(true) {
        bool state_changed{false};
        vector<string> next_state{iterate_visible(grid_visible, state_changed)};

        if(state_changed) {
            grid_visible = next_state;
        } else {
            break;
        }
    }

    int occupied_seats_visible{0};
    for_each(grid_visible.begin(), grid_visible.end(), [&occupied_seats_visible](string s){
        occupied_seats_visible += count(s.begin(), s.end(), '#');
    });

    cout << "Found the steady state for the visibility variant, seats occupied: " << occupied_seats_visible << endl;
}