// PS: This file for testing only

// Fundamental includes
#include <iostream>
#include <string>

// Json includes
#include "src/lib/json.hpp"
#include <fstream>
using json = nlohmann::json;

using namespace std;

int main() {

    system("cls");

    // [Read JSON File]
    ifstream readJson("src/data/test.json");

    if (!readJson.is_open()) {
        cout << "Unable to open file" << endl;
        return 1;
    }
    json data;
    readJson >> data;

    string name = data["question"][0]["title"];

    cout << name << endl;

    // [Write JSON File]
    ofstream writeJson("src/data/write.json");
    writeJson << setw(4) << data << endl;

    return 0;
       
}