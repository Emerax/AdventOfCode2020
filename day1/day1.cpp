#include <iostream>
#include <fstream>
#include <set>

using namespace std;

int main() {
    set<int> entries;

    //Read input.txt
    
    string input;
    ifstream file ("input.txt");

    if (file.is_open()) {
        while(getline(file, input)){
            entries.insert(stoi(input));
        }
        file.close();
    }
    
    int twoValueExpanseReport;
    for(auto it = entries.begin(); it != entries.end(); ++it){
        //Find sought-after value.
 
        if(entries.find(2020 - *it) != entries.end()) {
            twoValueExpanseReport = (2020 - *it) * (*it);
        }
    }

    int threeValueExpanseReport;
    for(auto first = entries.begin(); first != entries.end(); ++first) {
        for (auto second = next(first); second != entries.end(); ++second){

            if(entries.find(2020 - *first - *second) != entries.end()) {
                threeValueExpanseReport = (2020 - *first - *second) * (*first) * (*second);
            }
        }
    }

    
    std::cout << "Two-entry expense report: " << twoValueExpanseReport << endl;
    std::cout << "Three-entry expanse report: " << threeValueExpanseReport << endl;
}