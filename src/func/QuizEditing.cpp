#include <iostream>
#include <vector>
#include <conio.h> // สำหรับ _getch()

#include <windows.h>
#include <string>
#include <fstream>
#include <iomanip> // For setw

#include "../lib/json.hpp"
using json = nlohmann::json;

#include "../header/essential.h"

using namespace std;

void EditQuizInfo(json fileData, string filepath, string editOptions);
void EditQuizQuestions(json fileData, string filepath);

void QuizEditing() {

    while (true) {

        // Get the list of JSON files and add a "Back" option
        vector<string> jsonFileList = listJsonFile();
        jsonFileList.push_back("Back");

        // Let the user select a file to edit
        int choice = choiceSelection(
            "[Quiz Editing] \n\nPlease select the file you would like to edit.\n", 
            jsonFileList
        );

        // Check if the user selected "Back"
        if (choice == static_cast<int>(jsonFileList.size()) - 1) {
            return;
        }

        system("cls"); // Clear the screen

        // Prepare to read the selected JSON file
        string filepath = "src/data/" + jsonFileList[choice];
        ifstream readJson(filepath);

        // Check if the JSON file opened successfully
        if (!readJson.is_open()) {
            cout << "Unable to open file." << endl;
            continue;
        }

        // Check if the JSON file is empty
        readJson.seekg(0, ios::end);
        if (readJson.tellg() == 0) {
            cout << "JSON file is empty." << endl;
            cout << "Press any key to continue...";
            readJson.close();
            _getch();
            continue;
        }

        readJson.seekg(0, ios::beg); // Reset the file pointer for reading

        // Read the JSON data
        json data;
        readJson >> data;
        readJson.close();

        // Validate the JSON structure
        if (!data.contains("author") || !data.contains("title") || 
            !data.contains("description") || !data.contains("questionList")) {
            cout << "JSON file structure is invalid..." << endl;
            cout << "Press any key to continue...";
            _getch();
            continue;
        }

        while (true) {

            // Let the user choose what to edit
            int editChoice = choiceSelection(
                "[Quiz Editing] \n\nWhat would you like to edit? \n",
                {
                    "Author: " + to_string(data["author"]),
                    "Quiz title: " + to_string(data["title"]),
                    "Description: " + to_string(data["description"]),
                    "Quiz questions (total = " + to_string(data["questionList"].size()) + ")",
                    "Back"
                }
            );

            switch (editChoice) {
                case 0: // Edit the author
                    EditQuizInfo(data, filepath, "author");
                    break;
                case 1: // Edit the title
                    EditQuizInfo(data, filepath, "title");
                    break;
                case 2: // Edit the description
                    EditQuizInfo(data, filepath, "description");
                    break;
                case 3: // Edit the quiz questions
                    EditQuizQuestions(data, filepath);
                    return;
                default: // Exit if "Back" or any unexpected choice
                    return;
            }

        }


    }

    _getch();
    cout << "Press any key to continue..." << endl;
}

void EditQuizInfo(json fileData, string filepath, string editOptions) {

    string currentData = "";
    if (editOptions == "author") currentData = fileData["author"];
    else if (editOptions == "title") currentData = fileData["title"];
    else if (editOptions == "description") currentData = fileData["description"];

    string newData = "";

    while (true) {

        system("cls");

        cout << "[Quiz Editing]" << endl << endl;
        cout << "Current " << editOptions << ": " << currentData << endl;

        cout << "Enter new " << editOptions << " (Type '-' for cancel): ";
        getline(cin, newData);

        // If user cancelled
        if (newData[0] == '-') return;

        // Validate: Author input
        if (newData.empty()) {
            cout << endl << "Failed to update, the " << editOptions << " cannot be empty." << endl;
            cout << "Press any key to continue..." << endl;
            _getch();
            continue;
        }

        // If confirm & save change
        if (
            choiceSelection(
                "[Quiz Editing]\n\nCurrent " + editOptions + ": " + currentData + "\nNew " + editOptions + ": " + newData + "\n\nAre you sure to change and update?",
                {
                    "Confirm",
                    "Cancel"
                }
            ) == 0
        ) {

            // Json writing
            if (editOptions == "author") fileData["author"] = newData;
            else if (editOptions == "title") fileData["title"] = newData;
            else if (editOptions == "description") fileData["description"] = newData;

            ofstream writeJson(filepath);
            writeJson << setw(4) << fileData << endl;

            system("cls");
            cout << "[Quiz Editing]" << endl << endl;
            cout << "Update data completed!" << endl;
            cout << "Press any key to continue..." << endl;
            _getch();
            return;

        } else continue;

    }

    return;

}

void EditQuizQuestions(json fileData, const string filepath) {

    json questionList = fileData["questionList"];

    while (true) {
        int action = choiceSelection(
            "[Quiz Editing]\n\nQuiz: " + to_string(fileData["title"]) + "\n\nWhat would you like to do with this quiz?\n",
            {
                "Add new questions",
                "Edit questions",
                "Delete questions",
                "Back"
            }
        );

        if (action == 3) return; // Back

        system("cls");

        if (action == 0) { // Add new questions
            cout << "[Quiz - Editing]\n\n-= Add new questions =-\n\n";

            string title, answer, difficulty;
            int point = 0;
            string choices[4] = {"", "", "", ""};

            // Set the question title
            cout << "* Enter the question title: ";
            getline(cin, title);

            // Set the choices
            cout << "** Adding choices (Minimum 2, Maximum 4)\n";
            for (int i = 0; i < 4; ++i) {
                if (i < 2 || (choiceSelection("[Quiz - Editing]\n\n-= Add new questions =-\n\n* Adding more choices?\n", {"Yes", "No"}) == 0)) {
                    cout << "* Enter choice [" << char('A' + i) << "]: ";
                    getline(cin, choices[i]);
                } else {
                    break;
                }
            }

            // Set the correct answer
            bool isCorrectAnswer = false;
            while (!isCorrectAnswer) {
                int answerChoice = choiceSelection(
                    "[Quiz - Editing]\n\n-= Add new questions =-\n\n* Set the answer for this question?\n",
                    {
                        "[A] " + choices[0],
                        "[B] " + choices[1],
                        choices[2].empty() ? "None" : "[C] " + choices[2],
                        choices[3].empty() ? "None" : "[D] " + choices[3]
                    }
                );

                if (!choices[answerChoice].empty()) {
                    answer = char('A' + answerChoice);
                    isCorrectAnswer = true;
                } else {
                    cout << "Invalid answer, please try again." << endl;
                    _getch();
                }
            }

            // Set the question point
            cout << "* Set the point value for this question: ";
            cin >> point;
            cin.ignore(); // Clear the newline character left in the input buffer

            // Set the difficulty level
            difficulty = choiceSelection(
                "[Quiz - Editing]\n\n-= Add new questions =-\n\n* Set the difficulty of this question?\n",
                {"easy", "normal", "hard"}
            ) == 0 ? "easy" : (difficulty == "normal" ? "normal" : "hard");

            // Confirmation
            int confirmation = choiceSelection(
                "[Quiz - Editing]\n\n-= Add new questions =-\n\n* Review your question\n"
                "- Title: " + title + "\n"
                "- [A] " + choices[0] + "\n"
                "- [B] " + choices[1] + "\n"
                "- [C] " + choices[2] + "\n"
                "- [D] " + choices[3] + "\n",
                {"Confirm", "Cancel"}
            );

            if (confirmation == 0) { // Confirm
                json newQuestion = {
                    {"no", questionList.size() + 1},
                    {"title", title},
                    {"point", point},
                    {"difficulty", difficulty},
                    {"question", {
                        {"A", choices[0]},
                        {"B", choices[1]},
                        {"C", choices[2]},
                        {"D", choices[3]},
                        {"answer", answer}
                    }}
                };

                questionList.push_back(newQuestion);
                fileData["questionList"] = (questionList);

                ofstream writeJson(filepath);
                writeJson << setw(4) << fileData << endl;

                cout << "* Add new question completed!" << endl << endl;
                cout << "Press any key to continue..." << endl;
                _getch();
            } else { // Cancel
                cout << "Canceling... (Press any key to continue)" << endl;
                _getch();
            }

        } else if (action == 1) { // Edit questions
            cout << "You chose to edit a question" << endl;
            _getch();

        } else if (action == 2) { // Delete questions

            vector<string> questionTitleList;
            for (int i = 0; i < static_cast<int>(questionList.size()); i++) {

                string tmpQuestionTitle = "[" + to_string(i + 1) + "] " + questionList[i]["title"].dump();

                questionTitleList.push_back(tmpQuestionTitle);
            }

            int questionDeleteIndex = choiceSelection(
                "[Quiz - Editing]\n\n-= Delete the questions =-\n\n"
                "* Select the question that you would like to delete.\n",
                questionTitleList
            );

            int deleteConfirmation = choiceSelection(
                "[Quiz - Editing]\n\n-= Delete the questions =-\n\n"
                "* Are you sure to delete this question?\n"
                "\t Question: " + questionList[questionDeleteIndex]["title"].dump() + "\n",
                {"Confirm", "Cancel"}   // Confirm = 0, Cancel = 1
            );

            if (deleteConfirmation == 1) {  // If user cancelled
                cout << "Canceling... (Press any key to continue)" << endl;
                _getch();  
            } else {
                // Delete the question (by using index)
                questionList.erase(questionList.begin() + questionDeleteIndex);

                // Sorting the question number
                for (int i = 0; i < static_cast<int>(questionList.size()); i++) {
                    questionList[i]["no"] = i + 1;
                }
                
                // Update JSON File
                fileData["questionList"] = (questionList);
                ofstream writeJson(filepath);
                writeJson << setw(4) << fileData << endl;

                cout << "* Delete question completed!" << endl << endl;
                cout << "Press any key to continue..." << endl;
                _getch();
            }
        }
    }
}