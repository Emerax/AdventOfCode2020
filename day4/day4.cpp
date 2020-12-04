#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <regex>
#include <unordered_set>

//OBSERVE: In order to parse the entire input data, make sure to add two empty lines to it.

using namespace std;

vector<string> field_names {"byr:", "iyr:", "eyr:", "hgt:", "hcl:", "ecl:", "pid:"};

int valid_passport(map<string, string> pass){
    //Check byr
    if (pass["byr:"].empty() || pass["byr:"].size() != 4) {
        return false;
    } else {
        try {
            int year = stoi(pass["byr:"]);
            if (year < 1920 || year > 2002) {
                return false;
            }
        } catch (...) {
            return false;
        }
    }
    
    //Check iyr
    if (pass["iyr:"].empty() || pass["iyr:"].size() != 4) {
        return false;
    } else {
        try {
            int year = stoi(pass["iyr:"]);
            if (year < 2010 || year > 2020) {
                return false;
            }
        } catch (...) {
            return false;
        }
    }

    //Check eyr
    if (pass["eyr:"].empty() || pass["eyr:"].size() != 4) {
        return false;
    } else {
        try {
            int year = stoi(pass["eyr:"]);
            if (year < 2020 || year > 2030) {
                return false;
            }
        } catch (...) {
            return false;
        }
    }

    //Check hgt
    if (pass["hgt:"].empty()){
        return false;
    } else {
        regex re("[0-9]+(in|cm)");
        if (!regex_match(pass["hgt:"], re)) {
            return false;
        } else {
            int height = stoi(pass["hgt:"].substr(0, pass["hgt:"].size() - 2));
            if (pass["hgt:"].substr(pass["hgt:"].size() - 2) == "cm") {
                if (height < 150 || height > 193) {
                    return false;
                }
            } else {
                if (height < 59 || height > 76) {
                    return false;
                }
            }
        }
    }

    //Check hcl
    if (pass["hcl:"].empty()) {
        return false;
    } else {
        regex re("#([0-9]|[a-f]){6}");
        if (!regex_match(pass["hcl:"], re)){
            return false;
        }
    }

    //Check ecl
    unordered_set<string> colors{"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
    if (pass["ecl:"].empty() || colors.find(pass["ecl:"]) == colors.end()){
        return false;
    }

    //Check pid
    if (pass["pid:"].empty()){
        return false;
    } else {
        regex re("[0-9]{9}");
        if (!regex_match(pass["pid:"], re)) {
            return false;
        }
    }

    return true;
}

int main() {
    string input;
    ifstream file ("input.txt");

    map<string, string> passport{};
    int total_valid{0};
    int strictly_valid{0};
    
    if (file.is_open()) {
        while(getline(file, input)){
            if (input.empty()){
                //Check valid
                int valid{1};
                for (auto field : field_names) {
                    if (passport[field].empty()) {
                        valid = 0;
                        break;                        
                    } 
                }

                if (valid) {
                    ++total_valid;
                }

                if (valid_passport(passport)) {
                    ++strictly_valid;
                }

                //Clear passport for future use.
                passport = {};
            } else {
                //Parse fields, put in struct.
                int field_start{0};
                int field_end{0};
                for (auto field : field_names){
                    field_start = input.find(field);
                    if (field_start != string::npos) {
                        field_end = input.find(" ", field_start);
                        passport[field] = input.substr(field_start + 4, field_end - (field_start + 4));
                    }
                }
            }
        }
        file.close();
    }

    cout << "Found " << total_valid << " \"valid\" passports!" << endl;
    cout << "Found " << strictly_valid << " \"extra valid\" passports!" << endl;
}