// Quiz Editing (Poorin's Part)

#include <iostream>
#include <vector>
#include <conio.h> // สำหรับ _getch()

#include <windows.h>
#include <string.h>

// Json includes
#include "../lib/json.hpp"
#include <fstream>
using json = nlohmann::json;

#include "../header/essential.h"

using namespace std;

void QuizEditing() {

    while (true) {

        vector<string> jsonFileList = listJsonFile();
        jsonFileList.push_back("Back");

        int choice = choiceSelection(
            "[Quiz Editing] \nPlease select the file you want to edit.\n", 
            jsonFileList
        );

        if (choice != static_cast<int>(jsonFileList.size()) - 1) {

            system("cls");

            // [Read JSON File]
            string filepath = "src/data/" + jsonFileList[choice];
            ifstream readJson(filepath);
            if (!readJson.is_open()) {
                cout << "Unable to open file" << endl;
                return;
            }
            json data;
            readJson >> data;

            cout << "[Quiz Data]" << endl << endl;

            cout << "Filename: " << jsonFileList[choice] << endl;
            cout << "Quiz ID: " << generateUUID() << endl;
            cout << "Author: " << data["author"] << endl;
            cout << "Quiz title: " << data["title"] << endl;
            cout << "Description: " << data["description"] << endl;
            cout << "Number of quiz: " << static_cast<int>(data["questionList"].size()) << endl;

            _getch();
            cout << "Press any key to continue..." << endl;
        } else return;

    }

    _getch();
    cout << "Press any key to continue..." << endl;

    return;

}