#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "../header/essential.h"
using namespace std;

string createJSON(const string &quizId, const string &author, const string &title, const string &description, const string &diff, const string &questionList)
{
    string json = "\n";
    json += "{ \n";
    json += "  \"quizId\": \"" + quizId + "\",\n";
    json += "  \"author\": \"" + author + "\",\n";
    json += "  \"title\": \"" + title + "\",\n";
    json += "  \"description\": \"" + description + "\",\n";
    json += "  \"difficulty\": \"" + diff + "\",\n";
    json += "  \"questionList\": " + questionList + "\n";
    json += "}\n";
    return json;
}

string createQuestionList(int numQuestions, string title)
{
    vector<string> difficultyOptions = {"Easy", "Normal", "Hard"};
    string answerOptions[4] = {"A", "B", "C", "D"};

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
        while (true)
        {
            cout << "[Quiz no. " << i + 1 << " of " << numQuestions << " ]" << endl;

            cout << "Question " << i + 1 << " (or type 'cancel' to cancel): ";
            getline(cin, title);
            int checkspace = title.length();
            // check spacebar //ถึงตรงนี้

            // NULL
            if (checkspace == 0)
            {
                system("cls");
                cout << "Can't NULL Please Enter Question. \n ";
                continue;
            }

            if (title == "cancel")
                return "";
            break;
        }

        while (true)
        {
            string numChoicesInput;
            bool checkNum = true;
            cout << "Enter the number of choices (Minimum 2, Maximum 4): ";
            getline(cin, numChoicesInput);

            int checkspace = numChoicesInput.length();
            if (checkspace == 0)
            {
                system("cls");
                cout << "Can't NULL Please Enter Number of Choices. \n";
                continue;
            }

            for (char check : numChoicesInput)
            {
                if (!isdigit(check))
                {
                    checkNum = false;
                    break;
                }
            }

            if (checkNum == false)
            {
                system("cls");
                cout << "Invalid Number is Text. Plese enter Number.\n";
                continue;
            }

            numChoices = stoi(numChoicesInput);

            if (numChoices < 2 || numChoices > 4)
            {
                system("cls");
                cout << "Plese enter number of choices 2 - 4\n";
                continue;
            }
            break;
        }

        for (int j = 0; j < numChoices; ++j)
        {
            string option;
            while (true)
            {
                cout << "Enter Option " << answerOptions[j] << ": "; //[a,b,c,d]
                getline(cin, option);
                int checkspace = option.length();
                if (checkspace == 0)
                {
                    system("cls");
                    cout << "Can't NULL Please Enter Option. \n";
                    continue;
                }
                break;
            }

            options.push_back(option);
        }

        vector<string> displayOptions;
        for (int j = 0; j < numChoices; ++j)
        {
            displayOptions.push_back(answerOptions[j] + ": " + options[j]); // A : 1
        }

        int answerIndex = choiceSelection("Select the Correct Answer for question " + title + ":", displayOptions);

        string answer = answerOptions[answerIndex];

        int difficulty = choiceSelection("Select Difficulty for question " + title + ":", difficultyOptions) + 1;
        difficultyStr = difficultyOptions[difficulty - 1];
        while (true)
        {
            bool checkPoint = true;
            string pointStr;
            cout << "Enter Point Value: ";
            getline(cin, pointStr);
            int lenPoint = pointStr.length();

            if (lenPoint == 0)
            {
                system("cls");
                cout << "Point is Can't NULL.\n";
                continue;
            }

            for (char check : pointStr)
            {
                if (!isdigit(check))
                {
                    checkPoint = false;
                    break;
                }
            }

            if (checkPoint == false)
            {
                system("cls");
                cout << "Invalid Number is Text. Plese enter Number.\n";
                continue;
            }

            point = stoi(pointStr);

            if (point < 0)
            {
                system("cls");
                cout << "Invalid point value. Operation canceled.\n";
                continue;
            }
            break;
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

string convertFilename(const string &title, const string &quizId)
{
    string filename = title + "_" + quizId;

    return filename + ".json";
}

void QuizCreating()
{
    system("cls");

    cout << "[Theerapat's Modules] \n\n";
    cout << "Quiz Creating" << endl;

    string quizId = generateUUID();
    string author, title, description, diff;

    while (true)
    {
        cout << "Quiz ID generated: " << quizId << endl;

        while (true)
        {
            cout << "Enter the quiz title: ";
            getline(cin, title);
            int len = title.length();
            if (len == 0)
            {
                system("cls");
                cout << "Tile Can't NULL Plese Enter Text Title." << endl;
                continue;
            }
            break;
        }

        cout << "Enter the quiz description (Optional): ";
        getline(cin, description);

        while (true)
        {
            cout << "Enter the author name: ";
            getline(cin, author);
            int len = author.length();
            if (len == 0)
            {
                system("cls");
                cout << "Author Can't NULL Plese Enter Text Name.\n";
                continue;
            }
            break;
        }

        int diffQuiz = choiceSelection("Select the Quiz Difficulty:", {"Easy", "Normal", "Hard"});

        if (diffQuiz == 0)
        {
            diff = "Easy";
        }
        else if (diffQuiz == 1)
        {
            diff = "Normal";
        }
        else
        {
            diff = "Hard";
        }

        int numQuestions;
        while (true)
        {
            string numQuestionInputs;
            bool checkNum = true;
            cout << "Enter the number of questions (Maximum 100): ";
            getline(cin, numQuestionInputs);

            int checkspace = numQuestionInputs.length();
            if (checkspace == 0)
            {
                system("cls");
                cout << "Can't NULL \n";
                continue;
            }

            for (char check : numQuestionInputs)
            {
                if (!isdigit(check))
                {
                    checkNum = false;
                    break;
                }
            }

            if (checkNum == false)
            {
                system("cls");
                cout << "Invalid Number is Text. Plese enter Number.\n";
                continue;
            }

            numQuestions = stoi(numQuestionInputs);

            if (numQuestions > 100 || numQuestions < 1)
            {
                system("cls");
                cout << "Invalid Number is Range. Please 1-100.\n";
                continue;
            }
            break;
        }

        string questionList = createQuestionList(numQuestions, title);
        if (questionList == "")
        {
            return;
        }
        string json = createJSON(quizId, author, title, description, diff, questionList);

        string filename = convertFilename(title, quizId);
        string path = "src/data/" + filename;

        
        ofstream file(path);
        if (file.is_open())
        {
            file << json;
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
