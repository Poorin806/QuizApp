#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "../lib/json.hpp"
#include "../header/essential.h"

using json = nlohmann::json;
using namespace std;

void QuizTaking() {

    system("cls");
    int totalScore = 0;
    int lastScore = 0;
    double percentage = 0.0; 
    string testTitle, testDescription, testAuthor,testdif;
    int totalPoints = 0;

    while (true) {
            vector<string> jsonFileList = listJsonFile();
        jsonFileList.push_back("Back");

        int choice = choiceSelection(
            "[File Select] \nPlease select the file you want to do.\n", 
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
            json quizData;
            readJson >> quizData;
        
      

       
        testTitle = quizData["title"]; 
        testDescription = quizData["description"];
        testAuthor = quizData["author"];
        testdif = quizData["difficulty of Quiz"];
        int totalQuiz = quizData["questionList"].size();
        totalPoints = 0;
        for (const auto& questionItem : quizData["questionList"]) {
            totalPoints += questionItem["point"].get<int>(); // 
        }
        cout << "[QUIZ - Taking a test]\n\n";
        cout << "** " << testTitle << " **\n";
        cout << "Description: " << testDescription << endl;
        cout << "Author: " << testAuthor << endl;
        cout << "Total Quiz: " << totalQuiz << " (" << totalPoints << " Points)" << endl;
        cout << "Difficulty: " << testdif << endl;
        cout << "   \n" ;
        system("pause");
        system("cls");
        totalScore = 0;
        for (const auto& questionItem : quizData["questionList"]) {
            string no = to_string(questionItem["no"].get<int>());
            string questionTitle = questionItem["title"];
            auto choices = questionItem["question"];
            string correctAnswer = choices["answer"].get<string>();
            int point = questionItem["point"];
            string dif = questionItem["difficulty"];

     
            vector<string> listChoice;
                if (choices.contains("A")&& !choices["A"].get<string>().empty()) {
        listChoice.push_back("[A] " + choices["A"].get<string>());
    }
                if (choices.contains("B")&& !choices["B"].get<string>().empty()) {
        listChoice.push_back("[B] " + choices["B"].get<string>());
    }
                if (choices.contains("C")&& !choices["C"].get<string>().empty()) {
        listChoice.push_back("[C] " + choices["C"].get<string>());
    }
                if (choices.contains("D")&& !choices["D"].get<string>().empty()) {
        listChoice.push_back("[D] " + choices["D"].get<string>());
    } 
            

            int choice = choiceSelection(
                "[Quiz App - Taking a test] \n\n" +
                no + ". " + questionTitle + "\n",
                listChoice 
            );
            system("cls");

            // Check the answer
            char answerLetter = 'A' + choice; // Convert index to corresponding letter (A, B, C, D)
            if (string(1, answerLetter) == correctAnswer) {
                lastScore = point;  // Update the latest score with the point of the current question
                totalScore += point;
                cout << "Correct answer!" << endl;
            } else {
                lastScore = 0;  // If the answer is incorrect, no points are awarded
                cout << "Incorrect answer!" << endl;
            }
            
            
            cout << "Your latest score is: " << lastScore << endl;
            cout << "Your total score is: " << totalScore << endl;
            cout << "You selected choice: \n" << listChoice[choice] << endl;
             cout << "   \n" ;
            system("pause");
            system("cls");
        }

        percentage = ((double)totalScore / totalPoints) * 100;

        string resultMessage;
        if (percentage >= 80) {
            resultMessage = "Excellent";
        } else if (percentage >= 50) {
            resultMessage = "Nice";
        } else {
            resultMessage = "you can try again";
        }

        cout << "[QUIZ - Taking a test]\n";
         cout << "---------------------------------------\n";
        cout << "** " << testTitle << " ** (Completed)\n";
        cout << "- Your score: " << totalScore << "/" << totalPoints << " (" << fixed << setprecision(2) << percentage << "%)\n";
        cout << "-= [ " << resultMessage << " ] =-" << endl;
        cout << "---------------------------------------\n";
         cout << "   \n" ;
        system("pause");
        system("cls");
 
        int nextAction = choiceSelection(
            "* What would you like to do?\n",
            {
                "Retake the test",
                "Back to the menu"
            }
        );

        if (nextAction == 0) {
            system("cls");
            continue; 
        } else {
            break; 
        }
    }
    else {
        break;
    }
}
}
