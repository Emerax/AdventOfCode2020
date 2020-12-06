#include <iostream>
#include <fstream>
#include <set>

using namespace std;

int main() {
    string input;
    ifstream file ("input.txt");
    set<char> current_group_answers{};
    set<char> current_group_agreements{};

    int answers{0};
    int agreements{0};
    int first{true};

    if (file.is_open()) {
        while(getline(file, input)){
            if (input.empty()) {
                //Done with the previous group, move on.
                answers += current_group_answers.size();
                agreements += current_group_agreements.size();

                cout << "Group contained: ";
                for (auto c : current_group_agreements) {
                    cout << c << ", ";
                }
                cout << endl;
                cout << "Added " << current_group_agreements.size() << " to agreements" << endl;

                current_group_answers.clear();
                current_group_agreements.clear();
                first = true;
            } else {
                //Add answers to group;
                for (auto answer : input) {
                    current_group_answers.insert(answer);
                }

                if (first) {
                    first = false;
                    for (auto answer: input) {
                        current_group_agreements.insert(answer);
                    }

                } else {
                    set<char> new_agreements{};
                    for (auto it{current_group_agreements.begin()}; it != current_group_agreements.end(); ++it) {
                        if (input.find(*it) != string::npos) {
                            new_agreements.insert(*it);
                        }
                    }
                    current_group_agreements = new_agreements;
                }
            }
        }
        file.close();
    }

    cout << "Counted: " << answers << " total answers!" << endl;
    cout << "Counted: " << agreements << " total answers where groups agreed!" << endl;
}