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

    // // [Read JSON File]
    // ifstream readJson("src/data/test.json");

    // if (!readJson.is_open()) {
    //     cout << "Unable to open file" << endl;
    //     return 1;
    // }
    // json data;
    // readJson >> data;

    // string name = data["question"][0]["title"];

    // cout << name << endl;

    // [Write JSON File]
    json mockupWriteJsonData = {
        {"id", "UUID_4_ID"},
        {"author", "Poorin Thatsakhon"},
        {"title", "Quiz title (Exapmle)"},
        {"description", ""},
        {"questionList", {
            {
                {"no", 1},
                {"title", "What's this?"},
                {"question", {
                    {"A", "This is A"},
                    {"B", "This is B"},
                    {"C", "This is C"},
                    {"D", "This is D"},
                    {"answer", "A"}
                }},
                {"difficulty", "easy"},
                {"point", 1}
            }
        }}
    };
    ofstream writeJson("src/data/testWriteJsonFile.json");
    writeJson << setw(4) << mockupWriteJsonData << endl;

    return 0;
       
}