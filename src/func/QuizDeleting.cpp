#include <iostream>
#include <vector>
#include <filesystem>
#include <conio.h>
#include "../header/essential.h"

namespace fs = std::filesystem;
using namespace std;

string selectFileFromDirectory(const fs::path &directory)
{

    vector<string> jsonFiles = listJsonFile();
    vector<string> fileNames = jsonFiles;

    fileNames.push_back("Back");
    int choice = choiceSelection("Select a JSON file to delete:", fileNames);

    if (fileNames[choice] == "Back")
    {
        return "";
    }

    return jsonFiles[choice];
}

bool confirmDelete(const string &filename)
{
    vector<string> options = {"Yes", "No"};
    int choice = choiceSelection("Are you sure you want to delete " + filename + "?", options);

    return (choice == 0);
}

void deleteFile()
{
    fs::path directory = "src/data/";
    fs::path logdirectory = "src/data/log";

    string filepath = selectFileFromDirectory(directory);
    string logfilepath = filepath;

    if (filepath.empty())
    {
        cout << "No file selected or going back to the previous menu." << endl;
        return;
    }

    if (confirmDelete(filepath) == true)
    {
        try
        {
            if (fs::remove(directory / filepath))
            {
                cout << "File successfully deleted: " << filepath << endl;
            }
            else
            {
                cout << "File not found: " << filepath << endl;
            }

            if (fs::remove(logdirectory / logfilepath))
            {
                cout << "File successfully deleted: " << logfilepath << endl;
            }
            else
            {
                cout << "File not found: " << logfilepath << endl;
            }
        }
        catch (const fs::filesystem_error &e)
        {
            cerr << "Error deleting file: " << e.what() << endl;
        }
    }
}

void QuizDeleting()
{
    deleteFile();
}
