#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "../header/essential.h"
using namespace std;

// ฟังก์ชันสำหรับสร้าง JSON ในรูปแบบสตริง
string createJSON(const string &quizId, const string &author, const string &title, const string &description, const string &diff, const string &questionList)
{
    string json = "{\n";
    json += "  \"quizId\": \"" + quizId + "\",\n";
    json += "  \"author\": \"" + author + "\",\n";
    json += "  \"title\": \"" + title + "\",\n";
    json += "  \"description\": \"" + description + "\",\n";
    json += "  \"difficulty of Quiz\": \"" + diff + "\",\n";
    json += "  \"questionList\": " + questionList + "\n";
    json += "}\n";
    return json;
}

// ฟังก์ชันสำหรับสร้างรายการคำถามในรูปแบบ JSON
string createQuestionList(int numQuestions)
{
    vector<string> difficultyOptions = {"Easy", "Medium", "Hard"};
    vector<string> answerOptions = {"A", "B", "C", "D"};

    string questions = "[\n";
    for (int i = 0; i < numQuestions; ++i)
    {
        if (i > 0)
            questions += ",\n";
        questions += "  {\n";
        questions += "    \"no\": " + to_string(i + 1) + ",\n";
    
        string title, difficultyStr;
        int point, numChoices;
        vector<string> options;

        cout << "[Quiz no. " << i + 1 << " of " << numQuestions << " ]" << endl;

        cout << "Question " << i + 1 << " (or type 'cancel' to cancel): ";
        getline(cin, title);
        if (title == "cancel")
            return ""; // กรณีผู้ใช้ต้องการยกเลิก

        while (true)
        {
            cout << "Enter the number of choices (Minimum 2, Maximum 4): ";
            cin >> numChoices;
            cin.ignore();
            if (numChoices >= 2 && numChoices <= 4)
                break;
            cout << "Invalid number of choices. Please enter a valid number.\n";
        }

        for (int j = 0; j < numChoices; ++j)
        {
            string option;
            cout << "Enter Option " << answerOptions[j] << ": ";
            getline(cin, option);
            options.push_back(option);
        }

        int answerIndex = choiceSelection("Select the Correct Answer for question " + to_string(i + 1) + ":", vector<string>(answerOptions.begin(), answerOptions.begin() + numChoices));
        string answer = answerOptions[answerIndex];

        int difficulty = choiceSelection("Select Difficulty for question " + to_string(i + 1) + ":", difficultyOptions) + 1;
        difficultyStr = difficultyOptions[difficulty - 1];

        cout << "Enter Point Value: ";
        cin >> point;
        cin.ignore(); // ล้างบัฟเฟอร์ newline
        if (point < 0)
        {
            cout << "Invalid point value. Operation canceled.\n";
            return "";
        }

        questions += "    \"title\": \"" + title + "\",\n";
        questions += "    \"question\": {\n";
        for (int j = 0; j < numChoices; ++j)
        {
            questions += "      \"" + answerOptions[j] + "\": \"" + options[j] + "\"";
            questions += ",";
            questions += "\n";
        }
        questions += "      \"answer\": \"" + answer + "\"\n";
        questions += "    },\n";
        questions += "    \"difficulty\": \"" + difficultyStr + "\",\n";
        questions += "    \"point\": " + to_string(point) + "\n";
        questions += "  }";
    }
    questions += "\n]";
    return questions;
}

// ฟังก์ชันสำหรับแปลงชื่อไฟล์
string sanitizeFilename(const string &title, const string &quizId)
{
    string filename = title + "_" + quizId;
    // แทนที่ช่องว่างด้วย _
    replace(filename.begin(), filename.end(), ' ', '_');
    return filename + ".json";
}

void QuizCreating()
{
    system("cls");

    cout << "[Theerapat's Modules] \n\n";
    cout << "Quiz Creating" << endl;

    string quizId = generateUUID(); // Automatically generate UUID
    string author, title, description, diff;

    while (true)
    {
        cout << "Quiz ID generated: " << quizId << endl;
        cout << "Enter the quiz title: ";
        getline(cin, title);
        cout << "Enter the quiz description (Optional): ";
        getline(cin, description);
        cout << "Enter the author name: ";
        getline(cin, author);

        int diffQuiz = choiceSelection("Select the Quiz Difficulty:", {"Easy", "Medium", "Hard"});
        diff = (diffQuiz == 0) ? "Easy" : (diffQuiz == 1) ? "Medium" : "Hard";

        int numQuestions;
        cout << "Enter the number of questions (Maximum 100): ";
        cin >> numQuestions;
        cin.ignore();

        if (numQuestions > 100 || numQuestions < 1)
        {
            cout << "Invalid number of questions. Please enter a valid number.\n";
            continue;
        }
        // Create the list of questions
        string questionList = createQuestionList(numQuestions);

        if (questionList.empty())
        {
            cout << "Operation canceled. Returning to menu...\n";
            return; // Go back to the main menu
        }

        // Create JSON
        string json = createJSON(quizId, author, title, description, diff ,questionList);

        // Define the path where the file will be saved
        string filename = sanitizeFilename(title, quizId);
        string path = "src/data/" + filename;

        // Save JSON to file
        ofstream file(path);
        if (file.is_open())
        {
            file << json; // Write JSON to file
            file.close();
            cout << "JSON File successfully created at " << path << endl;
        }
        else
        {
            cout << "Can't open this file '" << path << "'" << endl;
        }

        cout << "Press any key to continue..." << endl;
        cin.get();

        return;
    }
}
