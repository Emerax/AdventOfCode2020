#include <iostream>
#include <fstream>
#include <map>
#include "day17.h"
#include <sstream>

using namespace std;

string print_slice(int z, map<coord, bool> space, pair<int, int> x_bounds, pair<int, int> y_bounds) {
    stringstream os;
    for (int y{y_bounds.first}; y <= y_bounds.second; ++y) {
        for (int x{x_bounds.first}; x <= x_bounds.second; ++x) {
            if (space[{x, y, z}]) {
                os << "#";
            } else {
                os << ".";
            }
        }
        os << "\n";
    }

    return os.str();
}

string print_neighbour_counts_slice(int z, map<coord, int> space, pair<int, int> x_bounds, pair<int, int> y_bounds) {
    stringstream os;
    for (int y{y_bounds.first}; y <= y_bounds.second; ++y) {
        for (int x{x_bounds.first}; x <= x_bounds.second; ++x) {
            os << space[{x, y, z}] << ", ";
        }
        os << "\n";
    }

    return os.str();
}

string print_bounds (pair<int, int> x_bounds, pair<int, int> y_bounds, pair<int, int> z_bounds) {
    stringstream os;
    os << "x: (" << x_bounds.first << ", " << x_bounds.second << "), y: (" << y_bounds.first << ", " << y_bounds.second << "), z: (" << z_bounds.first << ", " << z_bounds.second << ")";
    return os.str();
}

map<coord, bool> iterate(map<coord, bool> previous_state, pair<int, int> &x_bounds, pair<int, int> &y_bounds, pair<int, int> &z_bounds) {
    map<coord, int> neighbour_count{};

    --x_bounds.first;
    ++x_bounds.second;

    --y_bounds.first;
    ++y_bounds.second;

    --z_bounds.first;
    ++z_bounds.second;


    for (auto cell : previous_state) {
        if (cell.second) {
            int x{cell.first.x}, y{cell.first.y}, z{cell.first.z};
            for (int dz{-1}; dz <= 1; ++dz) {
                for (int dy{-1}; dy <= 1; ++dy) {
                    for (int dx{-1}; dx <= 1; ++dx) {
                        if (dx != 0 || dy != 0 || dz != 0) {
                            struct coord c{x + dx, y + dy, z + dz};
                            if (neighbour_count.find(c) == neighbour_count.end()) {
                                //First time visiting this cell, initialize it.
                                neighbour_count[c] = 1;
                            } else {
                                ++neighbour_count[c];
                            }
                        }
                    }
                }
            }
        }
    }

    map<coord, bool> next_state{};

    for (auto cell : neighbour_count) {
        if (previous_state[cell.first]) {
            if (cell.second == 2 || cell.second == 3) {
                next_state[cell.first] = true;
            } else {
                next_state[cell.first] = false;
            }
        } else {
            if (cell.second == 3) {
                next_state[cell.first] = true;
            } else {
                next_state[cell.first] = false;
            }
        }
    }

    return next_state;
}

map<hyper_coord, bool> hyperate(map<hyper_coord, bool> previous_state, pair<int, int> &x_bounds, pair<int, int> &y_bounds, pair<int, int> &z_bounds, pair<int, int> &w_bounds) {
    map<hyper_coord, int> neighbour_count{};

    --x_bounds.first;
    ++x_bounds.second;

    --y_bounds.first;
    ++y_bounds.second;

    --z_bounds.first;
    ++z_bounds.second;

    --w_bounds.first;
    ++w_bounds.second;

    for (auto cell : previous_state) {
        if (cell.second) {
            int x{cell.first.x}, y{cell.first.y}, z{cell.first.z}, w{cell.first.w};
            for (int dw{-1}; dw <= 1; ++dw) {
                for (int dz{-1}; dz <= 1; ++dz) {
                    for (int dy{-1}; dy <= 1; ++dy) {
                        for (int dx{-1}; dx <= 1; ++dx) {
                            if (dx != 0 || dy != 0 || dz != 0 || dw != 0) {
                                struct hyper_coord c{x + dx, y + dy, z + dz, w + dw};
                                if (neighbour_count.find(c) == neighbour_count.end()) {
                                    //First time visiting this cell, initialize it.
                                    neighbour_count[c] = 1;
                                } else {
                                    ++neighbour_count[c];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    map<hyper_coord, bool> next_state{};

    for (auto cell : neighbour_count) {
        if (previous_state[cell.first]) {
            if (cell.second == 2 || cell.second == 3) {
                next_state[cell.first] = true;
            } else {
                next_state[cell.first] = false;
            }
        } else {
            if (cell.second == 3) {
                next_state[cell.first] = true;
            } else {
                next_state[cell.first] = false;
            }
        }
    }

    return next_state;
}

int main() {
    ifstream file ("input.txt");
    string input{};

    map<coord, bool> space{};
    pair<int, int> x_bounds{0, 0};
    pair<int, int> y_bounds{0, 0};
    pair<int, int> z_bounds{0, 0};

    map<hyper_coord, bool> hyper_space{};
    pair<int, int> hyper_x_bounds{0, 0};
    pair<int, int> hyper_y_bounds{0, 0};
    pair<int, int> hyper_z_bounds{0, 0};
    pair<int, int> hyper_w_bounds{0, 0};

    if (file.is_open()) {
        int y{0}, z{0}, w{0};
        while(getline(file, input)) {
            int x{0};
            for (auto c : input) {
                if (c == '.') {
                    space.insert(pair<coord, bool>{{x, y, z}, false});
                    hyper_space.insert({{x, y, z, w}, false});
                } else {
                    space.insert(pair<coord, bool>{{x, y, z}, true});
                    hyper_space.insert({{x, y, z, w}, true});
                }
                if (x > x_bounds.second) {
                    x_bounds.second = x;
                    hyper_x_bounds.second = x;
                }
                ++x;
            }
            if (y > y_bounds.second) {
                y_bounds.second = y;
                hyper_y_bounds.second = y;
            }
            ++y;
        }
        file.close();

        for (int iteration{1}; iteration <= 6 ; ++iteration) {
            space = iterate(space, x_bounds, y_bounds, z_bounds);
        }

        int sum{0};
        for(auto cell : space) {
            if (cell.second) {
                ++sum;
            }
        }

        cout << "After 6 iterations, there are " << sum << " active cells" << endl;

        for (int iteration{1}; iteration <= 6; ++iteration) {
            hyper_space = hyperate(hyper_space, hyper_x_bounds, hyper_y_bounds, hyper_z_bounds, hyper_w_bounds);
        }

        int hyper_sum{0};
        for (auto hyper_cell : hyper_space) {
            if (hyper_cell.second) {
                ++hyper_sum;
            }
        }

        cout << "After 6 hyperations, there are " << hyper_sum << " hyper active hyper cells" << endl;
    }
}