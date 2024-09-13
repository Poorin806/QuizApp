#include <iostream>
#include <vector>
#include <filesystem>
#include <conio.h>
#include "../header/essential.h"

namespace fs = std::filesystem;
using namespace std;

// ฟังก์ชันเพื่อแสดงรายการไฟล์ JSON ในโฟลเดอร์และให้ผู้ใช้เลือกไฟล์
string selectFileFromDirectory(const fs::path &directory) {
    vector<fs::path> jsonFiles;
    vector<string> fileNames;

    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        cout << "Directory does not exist!" << endl;
        return "";
    }

    for (const auto &entry : fs::directory_iterator(directory)) {
        if (fs::is_regular_file(entry.status()) && entry.path().extension() == ".json") {
            jsonFiles.push_back(entry.path());
            fileNames.push_back(entry.path().filename().string());
        }
    }

    if (jsonFiles.empty()) {
        cout << "No JSON files found in the directory." << endl;
        return "";
    }

    // เพิ่มตัวเลือก "Back"
    fileNames.push_back("Back");

    // ใช้ choiceSelection เพื่อให้ผู้ใช้เลือกไฟล์
    int choice = choiceSelection("Select a JSON file to delete:", fileNames);

    if (choice == -1 || choice >=  static_cast<int>(fileNames.size())) {
        return "";
    }

    // ถ้าผู้ใช้เลือก "Back"
    if (choice == static_cast<int>(fileNames.size()) - 1) {
        return ""; 
    }

    return jsonFiles[choice].string();
}

bool confirmDeletion(const string &filename) {
    vector<string> options = {"Yes", "No"};
    int choice = choiceSelection("Are you sure you want to delete " + filename + "?", options);

    return (choice == 0); // ถ้าผู้ใช้เลือก "Yes", คืนค่า true
}

void deleteFile() {
    fs::path directory = "src/data/";
    string filepath = selectFileFromDirectory(directory);

    if (filepath.empty()) {
        cout << "No file selected or going back to the previous menu." << endl;
        return;
    }

    if (confirmDeletion(fs::path(filepath).filename().string())) {
        try {
            if (fs::remove(filepath)) {
                cout << "File successfully deleted: " << filepath << endl;
            } else {
                cout << "File not found: " << filepath << endl;
            }
        } catch (const fs::filesystem_error &e) {
            cerr << "Error deleting file: " << e.what() << endl;
        }
    } else {
        cout << "File deletion cancelled." << endl;
    }
}

void QuizDeleting() {
    deleteFile();
}
