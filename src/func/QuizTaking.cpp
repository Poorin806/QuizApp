#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "../lib/json.hpp"
#include "../header/essential.h"
#include <ctime>
#include  <iomanip>
#include <chrono>
#include <cstdlib>
#include <sstream>

using json = nlohmann::json;
using namespace std;

void QuizTaking() {

    srand(time(0));

    system("cls");
    int totalScore = 0;
    int lastScore = 0;
    double percentage = 0.0;
    string testId, testTitle, testDescription, testAuthor, testdif;
    int totalPoints = 0;

    while (true)
    {

        vector<string> jsonFileList = listJsonFile();
        jsonFileList.push_back("Back");

        // Show file lists (Quiz lists)
        int choice = choiceSelection(
            "[File Select] \nPlease select the file you want to do.\n",
            jsonFileList);
        if (choice != static_cast<int>(jsonFileList.size()) - 1)
        {

            system("cls");

            // [Read JSON File]
            string filepath = "src/data/" + jsonFileList[choice];
            ifstream readJson(filepath);
            if (!readJson.is_open())
            {
                cout << "Unable to open file" << endl;
                return;
            }
            json quizData;
            readJson >> quizData;

            // Quiz Data (User selected for do the test)
            testId = quizData["quizId"];
            testTitle = quizData["title"];
            testDescription = quizData["description"];
            testAuthor = quizData["author"];
            testdif = quizData["difficulty"];
            int totalQuiz = quizData["questionList"].size();
            totalPoints = 0;

            // Asking user for showing the quistion results when answer
            int isShowAnswer = choiceSelection(
                "[Quiz - Taking a test] \n\n"
                "Would you like to show the results when you answer the question?\n",
                {
                    "No",    // value = 0
                    "Yes"  // value = 1
                }
            );

            int isShuffleQuestion = choiceSelection(
                "[Quiz - Taking a test] \n\n"
                "Would you like to shuffle the question and choices in this quiz?\n",
                {
                    "No",    // value = 0
                    "Yes"  // value = 1
                }
            );

            system("cls");

            // Looping each question
            for (const auto &questionItem : quizData["questionList"])
            {
                totalPoints += questionItem["point"].get<int>(); //
            }
            cout << "[QUIZ - Taking a test]\n\n";
            cout << "** " << testTitle << " **\n";
            cout << "Description: " << testDescription << endl;
            cout << "Author: " << testAuthor << endl;
            cout << "Total Quiz: " << totalQuiz << " (" << totalPoints << " Points)" << endl;
            cout << "Difficulty: " << testdif << endl;
            cout << "   \n";
            system("pause");
            system("cls");
            totalScore = 0;

            int *listRandomNo = new int[totalQuiz];

            // Handling the suffle questions
            if (isShuffleQuestion == 1) {   // Shuffling the questions

                // Store quiz data
                for (int i = 0; i < totalQuiz; i++) {
                    listRandomNo[i] = i;
                }

                // Fisher-Yates shuffle algorithm
                for (int i = totalQuiz - 1; i > 0; i--) {
                    // สุ่มค่า index j ระหว่าง 0 ถึง i
                    int j = rand() % (i + 1);

                    // Swapping
                    int temp = listRandomNo[i];
                    listRandomNo[i] = listRandomNo[j];
                    listRandomNo[j] = temp;
                }

            }

            // Looping and displaying each question            
            for (int i = 0; i < totalQuiz; i++) {

                // Handling the shuffle options
                auto &questionItem = isShuffleQuestion ? quizData["questionList"][listRandomNo[i]]
                                           : quizData["questionList"][i];

                string no = to_string(questionItem["no"].get<int>());
                string questionTitle = questionItem["title"];
                auto choices = questionItem["question"];    // "A", "B", "C" (Optional), "D" (Optional)
                string correctAnswer = choices["answer"].get<string>();
                int point = questionItem["point"];
                string dif = questionItem["difficulty"];

                vector<string> listChoice;
                if (choices.contains("A") && !choices["A"].get<string>().empty())
                {
                    listChoice.push_back("[A] " + choices["A"].get<string>());
                }
                if (choices.contains("B") && !choices["B"].get<string>().empty())
                {
                    listChoice.push_back("[B] " + choices["B"].get<string>());
                }
                if (choices.contains("C") && !choices["C"].get<string>().empty())
                {
                    listChoice.push_back("[C] " + choices["C"].get<string>());
                }
                if (choices.contains("D") && !choices["D"].get<string>().empty())
                {
                    listChoice.push_back("[D] " + choices["D"].get<string>());
                }

                // Handling the shuffle choices
                if (isShuffleQuestion == 1) {
                    int n = listChoice.size();
                    for (int i = n - 1; i > 0; i--) {
                        // สุ่มค่า index j ระหว่าง 0 ถึง i
                        int j = rand() % (i + 1);

                        // Swapping
                        std::string temp = listChoice[i];
                        listChoice[i] = listChoice[j];
                        listChoice[j] = temp;
                    }
                }

                int choice = choiceSelection(
                    "[Quiz App - Taking a test] \n\n" +
                        no + ". " + questionTitle + "\n",

                    listChoice
                );

                system("cls");

                // Check the answer

                // char answerLetter = 'A' + choice; // Convert index to corresponding letter (A, B, C, D)
                char answerLetter = listChoice[choice][1];

                if (string(1, answerLetter) == correctAnswer)
                {
                    lastScore = point; // Update the latest score with the point of the current question
                    totalScore += point;

                    // Handling show answers options
                    if (isShowAnswer == 1) cout << "Correct answer!" << endl;
                }
                else
                {
                    lastScore = 0; // If the answer is incorrect, no points are awarded
                    
                    // Handling show answers options
                    if (isShowAnswer == 1) cout << "Incorrect answer!" << endl;
                }

                if (isShowAnswer == 1) {
                    cout << "Your latest score is: " << lastScore << endl;
                    cout << "Your total score is: " << totalScore << endl;
                    cout << "You selected choice: \n"
                        << listChoice[choice] << endl;
                    cout << "   \n";
                }

                system("pause");
                system("cls");
            }

            delete [] listRandomNo;

            percentage = ((double)totalScore / totalPoints) * 100;

            string resultMessage;
            if (percentage >= 80)
            {
                resultMessage = "Excellent";
            }
            else if (percentage >= 50)
            {
                resultMessage = "Nice";
            }
            else
            {
                resultMessage = "you can try again";
            }

            cout << "[QUIZ - Taking a test]\n";
            cout << "---------------------------------------\n";
            cout << "** " << testTitle << " ** (Completed)\n";
            cout << "- Your score: " << totalScore << "/" << totalPoints << " (" << fixed << setprecision(2) << percentage << "%)\n";
            cout << "-= [ " << resultMessage << " ] =-" << endl;
            cout << "---------------------------------------\n";
            cout << "   \n";
            system("pause");
            system("cls");

            // Save the results in the log file
            auto now = chrono::system_clock::now();
            time_t currentTime = chrono::system_clock::to_time_t(now);
            tm *localTime = localtime(&currentTime);
            ostringstream timeStampStream;
            timeStampStream << put_time(localTime, "%Y-%m-%d %H:%M:%S");
            string timeStampFormatted = timeStampStream.str();

            json logData = {
                {"score", totalScore},
                {"timeStamp", timeStampFormatted}
            };

            string logFileName = "src/data/log/" + testId + ".json";

            // Handling if there is log file exists
            ifstream readJson_2(logFileName);
            json data;

            if (readJson_2.is_open()) {
                // Check if log file exists
                readJson_2 >> data;
                readJson_2.close();

                // Arrays Checking
                if (data.is_array()) {
                    data.push_back(logData);
                } else {
                    json newArray = json::array();
                    newArray.push_back(data);
                    newArray.push_back(logData);
                    data = newArray;
                }
            } else {
                // If create new file make a arrays
                data = json::array();
                data.push_back(logData);
            }

            // Save log into JSON
            ofstream writeJson(logFileName);
            writeJson << setw(4) << data << endl;
            writeJson.close();

            int nextAction = choiceSelection(
                "* What would you like to do?\n",
                {"Retake the test",
                 "Back to the menu"});

            if (nextAction == 0)
            {
                system("cls");
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
}
