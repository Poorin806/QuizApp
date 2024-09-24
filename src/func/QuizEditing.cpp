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

        // Json File Details
        while (true) {

            // Re-read the JSON file (In case of updating data)
            ifstream readJson(filepath);
            json data;
            readJson >> data;
            readJson.close();

            // Let the user choose what to edit
            int editChoice = choiceSelection(
                "[Quiz Editing] \n\nWhat would you like to edit? \n",
                {
                    "Author: " + to_string(data["author"]),
                    "Quiz title: " + to_string(data["title"]),
                    "Description: " + to_string(data["description"]),
                    "Quiz questions (total = " + to_string(data["questionList"].size()) + ")\n",
                    "Back"
                }
            );

            if (editChoice == 0) EditQuizInfo(data, filepath, "author");
            else if (editChoice == 1) EditQuizInfo(data, filepath, "title");
            else if (editChoice == 2) EditQuizInfo(data, filepath, "description");
            else if (editChoice == 3) EditQuizQuestions(data, filepath);
            else if (editChoice == 4) break; // Back

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
            
            vector<string> questionTitleList;
            for (int i = 0; i < static_cast<int>(questionList.size()); i++) {

                string tmpQuestionTitle = "[" + to_string(i + 1) + "] " + questionList[i]["title"].dump();
                if (i == 0) questionTitleList.push_back("[X] Back / Cancel\n");   // Index 0 = Back or Cancelled
                questionTitleList.push_back(tmpQuestionTitle);
            }

            int questionEditIndex = choiceSelection(
                "[Quiz - Editing]\n-= Edit the questions =-\n\n"
                "* Select the question that you would like to edit.\n",
                questionTitleList
            );

            // Check if user selected back or not
            if (questionEditIndex == 0) continue;
            else questionEditIndex--; // devalue by 1 for arrays (cause, by default 0 = back)

            string editQuestionTitle = questionList[questionEditIndex]["title"].get<string>();
            string editQuestionDifficulty = questionList[questionEditIndex]["difficulty"].get<string>();
            int editQuestionPoint = questionList[questionEditIndex]["point"].get<int>();

            string editQuestionChoicesA = questionList[questionEditIndex]["question"]["A"].get<string>();
            string editQuestionChoicesB = questionList[questionEditIndex]["question"]["B"].get<string>();
            string editQuestionChoicesC = questionList[questionEditIndex]["question"]["C"].get<string>();
            string editQuestionChoicesD = questionList[questionEditIndex]["question"]["D"].get<string>();
            string editQuestionChoicesAnswer = questionList[questionEditIndex]["question"]["answer"].get<string>();

            int questionEditType = choiceSelection(
                "[Quiz - Editing]\n\n-= Edit the questions =-\n\n"
                "* Select the question that you would like to edit.\n",
                {
                    "[X] Back / Cancel\n",
                    "Question Title: " + editQuestionTitle,
                    "[A]: " + editQuestionChoicesA,
                    "[B]: " + editQuestionChoicesB,
                    "[C]: " + ((editQuestionChoicesC == "" || editQuestionChoicesC.empty()) ? "None" : editQuestionChoicesC),
                    "[D]: " + ((editQuestionChoicesD == "" || editQuestionChoicesD.empty()) ? "None" : editQuestionChoicesD),
                    "Answer: " + editQuestionChoicesAnswer,
                    "Point: " + to_string(editQuestionPoint),
                    "Difficulty: " + editQuestionDifficulty
                }
            );

            if (questionEditType == 0) continue;    // If user didn't select question edit type

            // Question Title Edit
            if (questionEditType == 1) {

                string newQuestionTitle;

                cout << endl;
                cout << "* Enter new title (type \"-\" to cancel): ";
                getline(cin, newQuestionTitle);

                if (newQuestionTitle == "-") continue;
                
                // Confirmation
                int editConfirmation = choiceSelection(
                    "[Quiz - Editing]\n\n-= Edit the questions =-\n\n"
                    "* Are you sure to change this information?\n\n"
                    "\t - Current Question Title: " + editQuestionTitle + "\n"
                    "\t - New Question Title: " + newQuestionTitle + "\n\n",
                    {"Comfirm", "Cancel"}
                );
                if (editConfirmation == 1) continue;    // Cancel

                // Confirm
                questionList[questionEditIndex]["title"] = newQuestionTitle;
                fileData["questionList"] = (questionList);

                ofstream writeJson(filepath);
                writeJson << setw(4) << fileData << endl;

                cout << "* Edit question title completed!" << endl << endl;
                cout << "Press any key to continue..." << endl;

            }

            // Question Point Edit
            if (questionEditType == 7) {

                int newQuestionPoint = 0;

                cout << endl;
                cout << "* Enter new question point (type \"-1\" to cancel): ";
                cin >> newQuestionPoint;
                if (newQuestionPoint < 0) continue;

                // Confirmation
                int editConfirmation = choiceSelection(
                    "[Quiz - Editing]\n\n-= Edit the questions =-\n\n"
                    "* Are you sure to change this information?\n\n"
                    "\t - Current Question Point: " + to_string(editQuestionPoint) + "\n"
                    "\t - New Question Point: " + to_string(newQuestionPoint) + "\n\n",
                    {"Comfirm", "Cancel"}
                );
                if (editConfirmation == 1) continue;    // Cancel

                // Confirm
                questionList[questionEditIndex]["point"] = newQuestionPoint;
                fileData["questionList"] = (questionList);

                ofstream writeJson(filepath);
                writeJson << setw(4) << fileData << endl;

                cout << "* Edit question title completed!" << endl << endl;
                cout << "Press any key to continue..." << endl;

            }

            // Question Choice Edit (A-D)
            if (questionEditType >= 2 && questionEditType <= 5) {
                string newChoicesData;
                string currentEditChoice;
                string currentEditChoiceData;

                cout << endl;

                switch (questionEditType) {
                    case 2: {
                        // A
                        currentEditChoice = 'A';
                        currentEditChoiceData = editQuestionChoicesA;
                        cout << "* Enter new [A] question choice (type \"cancel\" to cancel): ";
                        break;
                    }
                    case 3: {
                        // B
                        currentEditChoice = 'B';
                        currentEditChoiceData = editQuestionChoicesB;
                        cout << "* Enter new [B] question choice (type \"cancel\" to cancel): ";
                        break;
                    }
                    case 4: {
                        // C
                        currentEditChoice = 'C';
                        currentEditChoiceData = editQuestionChoicesC;
                        cout << "* Enter new [C] question choice (type \"cancel\" to cancel): ";
                        break;
                    }
                    case 5: {
                        // D
                        currentEditChoice = 'D';
                        currentEditChoiceData = editQuestionChoicesD;
                        cout << "* Enter new [D] question choice (type \"cancel\" to cancel): ";
                        break;
                    }
                }

                getline(cin, newChoicesData);
                if (newChoicesData == "cancel") continue;

                // Confirmation
                int editConfirmation = choiceSelection(
                    "[Quiz - Editing]\n\n-= Edit the questions =-\n\n"
                    "* Are you sure to change this information?\n\n"
                    "\t - Current Question Choice [" + currentEditChoice + "]: " + currentEditChoiceData + "\n"
                    "\t - New Question Choice: [" + currentEditChoice + "]: " + newChoicesData + "\n\n",
                    {"Comfirm", "Cancel"}
                );
                if (editConfirmation == 1) continue;    // Cancel

                // Confirm
                questionList[questionEditIndex]["question"][currentEditChoice] = newChoicesData;
                fileData["questionList"] = (questionList);

                ofstream writeJson(filepath);
                writeJson << setw(4) << fileData << endl;

                cout << "* Edit question title completed!" << endl << endl;
                cout << "Press any key to continue..." << endl;

            }

            // Question Answer Edit
            if (questionEditType == 6) {

                string newAnswer;

                vector<string> currentChoice;
                int dynamicCancelChoice = 4;

                // Current Choice Checking
                if (editQuestionChoicesD.empty() || editQuestionChoicesD == "") {

                    // 2 Choices (A & B)
                    if (editQuestionChoicesC.empty() || editQuestionChoicesC == "") {
                        currentChoice.push_back("[A] " + editQuestionChoicesA);
                        currentChoice.push_back("[B] " + editQuestionChoicesB + "\n");
                        dynamicCancelChoice = 2;
                    } else {    // 3 Choices (A & B & C)
                        currentChoice.push_back("[A] " + editQuestionChoicesA);
                        currentChoice.push_back("[B] " + editQuestionChoicesB);
                        currentChoice.push_back("[C] " + editQuestionChoicesC + "\n");
                        dynamicCancelChoice = 3;
                    }

                } else {
                        currentChoice.push_back("[A] " + editQuestionChoicesA);
                        currentChoice.push_back("[B] " + editQuestionChoicesB);
                        currentChoice.push_back("[C] " + editQuestionChoicesC);
                        currentChoice.push_back("[D] " + editQuestionChoicesD + "\n");
                }

                currentChoice.push_back("Cancel");

                // Select (Edit) the answer
                int editDifficultyChoice = choiceSelection(
                    "[Quiz - Editing]\n\n-= Edit the questions =-\n\n"
                    "* Select the answer\n",
                    currentChoice
                );
                if (editDifficultyChoice == dynamicCancelChoice) continue;    // Cancel

                switch (editDifficultyChoice) {
                    case 0:
                        newAnswer = "A";
                        break;
                    case 1:
                        newAnswer = "B";
                        break;
                    case 2:
                        newAnswer = "C";
                        break;
                    case 3:
                        newAnswer = "D";
                        break;
                }

                // Confirmation
                int editConfirmation = choiceSelection(
                    "[Quiz - Editing]\n\n-= Edit the questions =-\n\n"
                    "* Are you sure to change this information?\n\n"
                    "\t - Current Answer: " + editQuestionChoicesAnswer + "\n"
                    "\t - New Answer: " + newAnswer + "\n\n",
                    {"Comfirm", "Cancel"}
                );
                if (editConfirmation == 1) continue;    // Cancel

                // Confirm
                questionList[questionEditIndex]["question"]["answer"] = newAnswer;
                fileData["questionList"] = (questionList);

                ofstream writeJson(filepath);
                writeJson << setw(4) << fileData << endl;

                cout << "* Edit question title completed!" << endl << endl;
                cout << "Press any key to continue..." << endl;

            }

            // Edit Question Difficulty
            if (questionEditType == 8) {

                string newDifficulty;

                // Select (Edit) the difficulty
                int editDifficultyChoice = choiceSelection(
                    "[Quiz - Editing]\n\n-= Edit the questions =-\n\n"
                    "* Select the difficulty\n",
                    { "Easy", "Normal", "Hard\n", "Cancel" }
                );
                if (editDifficultyChoice == 3) continue;    // Cancel

                switch (editDifficultyChoice) {
                    case 0:
                        newDifficulty = "easy";
                        break;
                    case 1:
                        newDifficulty = "normal";
                        break;
                    case 2:
                        newDifficulty = "hard";
                        break;
                }

                // Confirmation
                int editConfirmation = choiceSelection(
                    "[Quiz - Editing]\n\n-= Edit the questions =-\n\n"
                    "* Are you sure to change this information?\n\n"
                    "\t - Current Difficulty: " + editQuestionDifficulty + "\n"
                    "\t - New Difficulty: " + newDifficulty + "\n\n",
                    {"Comfirm", "Cancel"}
                );
                if (editConfirmation == 1) continue;    // Cancel

                // Confirm
                questionList[questionEditIndex]["difficulty"] = newDifficulty;
                fileData["questionList"] = (questionList);

                ofstream writeJson(filepath);
                writeJson << setw(4) << fileData << endl;

                cout << "* Edit question title completed!" << endl << endl;
                cout << "Press any key to continue..." << endl;

            }

            _getch();

        } else if (action == 2) { // Delete questions

            vector<string> questionTitleList;
            for (int i = 0; i < static_cast<int>(questionList.size()); i++) {

                string tmpQuestionTitle = "[" + to_string(i + 1) + "] " + questionList[i]["title"].dump();
                if (i == 0) questionTitleList.push_back("[X] Back / Cancel\n");   // Index 0 = Back or Cancelled
                questionTitleList.push_back(tmpQuestionTitle);
            }

            int questionDeleteIndex = choiceSelection(
                "[Quiz - Editing]\n\n-= Delete the questions =-\n\n"
                "* Select the question that you would like to delete.\n",
                questionTitleList
            );

            // Check if user selected back or not
            if (questionDeleteIndex == 0) continue;
            else questionDeleteIndex--; // devalue by 1 for arrays (cause, by default 0 = back)

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

    return;
}