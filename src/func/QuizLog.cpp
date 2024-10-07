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

void LogDetail(int quizIndex, vector<string> listJsonFiles);

void QuizLog() {

    vector<string> listJsonFiles = listJsonFile();

    // Formatted: raw json file -> quiz title
    vector<string> listQuizTitle = {};
    for (int i = 0; i < static_cast<int>(listJsonFiles.size()); i++) {

        // Prepare to read the selected JSON file
        string filepath = "src/data/" + listJsonFiles[i];
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

        string tmpData = data["title"].get<string>();

        listQuizTitle.push_back(
            (i == static_cast<int>(listJsonFiles.size()) - 1) ? tmpData + "\n" : tmpData
        );

    }

    // Add exit options
    listQuizTitle.push_back("Back");

    // Body
    while (true) {

        int quizSelectedIndex = choiceSelection(
            "[Quiz Taker & Maker]\n\n"
            "Choose the quiz to see the log:\n",
            listQuizTitle
        );

        // Handling exit options
        if (quizSelectedIndex == static_cast<int>(listQuizTitle.size()) - 1) break;

        // Show the quiz history (Detail)
        LogDetail(quizSelectedIndex, listJsonFiles);

    }

    return;

}

void LogDetail(int quizIndex, vector<string> listJsonFiles) {

    system("cls");

    // [Get Quiz Data]
    // Prepare to read the selected JSON file
    string filepath = "src/data/" + listJsonFiles[quizIndex];
    ifstream readJson(filepath);

    // Check if the JSON file opened successfully
    if (!readJson.is_open()) {
        cout << "Unable to open file." << endl;
        _getch();
        return;
    }

    // Check if the JSON file is empty
    readJson.seekg(0, ios::end);
    if (readJson.tellg() == 0) {
        cout << "JSON file is empty." << endl;
        cout << "Press any key to continue...";
        readJson.close();
        _getch();
        return;
    }

    readJson.seekg(0, ios::beg); // Reset the file pointer for reading

    // Read the JSON data
    json quizData;
    readJson >> quizData;
    readJson.close();

    // Get the total point of quiz
    int totalQuizPoint = 0;
    for (int i = 0; i < static_cast<int>(quizData["questionList"].size()); i++)
        totalQuizPoint += quizData["questionList"][i]["point"].get<int>();

    // [Show Quiz Details]
    cout << "[Quiz Taker & Maker]" << endl << endl;
    cout << "Quiz ID: " << quizData["quizId"].get<string>() << endl;
    cout << "Title: " << quizData["title"].get<string>() << endl;
    cout << "Description: " << ((quizData["description"].get<string>().length() <= 0) ? "None" : quizData["description"].get<string>()) << endl;
    cout << "Author: " << quizData["author"].get<string>() << endl;
    cout << "Difficulty: " << quizData["difficulty"].get<string>() << endl;
    cout << "Total Points: " << totalQuizPoint << endl;

    // [Get Log Data]
    // Prepare to read the selected JSON file
    string filepath_log = "src/data/log/" + quizData["quizId"].get<string>() + ".json";
    ifstream readJson_log(filepath_log);

    // Check if the JSON file opened successfully
    if (!readJson_log.is_open()) {
        cout << endl;
        cout << "Unable to open file. (There is no records in this quiz)" << endl;
        cout << endl;
        cout << "Press any key to continue...";
        _getch();
        return;
    }

    // Check if the JSON file is empty
    readJson_log.seekg(0, ios::end);
    if (readJson_log.tellg() == 0) {
        cout << "JSON file is empty." << endl;
        cout << "Press any key to continue...";
        readJson_log.close();
        _getch();
        return;
    }

    readJson_log.seekg(0, ios::beg); // Reset the file pointer for reading

    // Read the JSON data
    json logData;
    readJson_log >> logData;
    readJson_log.close();

    // Get the highest & lowest score
    int highestScoreIndex = -1;  // Stores the index of the highest score
    int lowestScoreIndex = -1;   // Stores the index of the lowest score
    int highestScore = -9999;    // Store the highest score itself
    int lowestScore = 9999;      // Store the lowest score itself
    for (int i = 0; i < static_cast<int>(logData.size()); i++) {
        int currentScore = logData[i]["score"].get<int>();

        // Check for highest score
        if (currentScore > highestScore) {
            highestScore = currentScore;
            highestScoreIndex = i; // Store the index
        }

        // Check for lowest score
        if (currentScore < lowestScore) {
            lowestScore = currentScore;
            lowestScoreIndex = i;  // Store the index
        }
    }

    cout << endl;

    // [Show Quiz Log]
    // Table Header
    cout << "-" << setw(64) << setfill('-') << "-" << setfill(' ') << endl;
        cout << "|" << setw(5) << right << "#" << "|";
        cout << setw(25) << right << "Score" << "|";
        cout << setw(30) << right << "Timestamp" << "|" << endl;
    cout << "-" << setw(64) << setfill('-') << "-" << setfill(' ') << endl;

    // Table Body
    for (int i = 0; i < static_cast<int>(logData.size()); i++) {

        string formattedScore = to_string(logData[i]["score"].get<int>());

        if (i == highestScoreIndex) {
            formattedScore += " (highest)";
        } else if (i == lowestScoreIndex) {
            formattedScore += " (lowest)";
        }

        // Row No.
        cout << "|" << setw(5) << right << to_string(i + 1) << "|";
        
        // Score
        cout << setw(25) << right << formattedScore << "|";

        // Timestamp
        cout << setw(30) << right << logData[i]["timeStamp"].get<string>() << "|" << endl;
    }

    cout << "-" << setw(64) << setfill('-') << "-" << setfill(' ') << endl;
    
    cout << endl;

    cout << "Press any key to continue...";

    _getch();

    return;

}