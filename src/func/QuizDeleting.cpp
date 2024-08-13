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
    int index = 1;

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

    if (choice == -1 || choice >= fileNames.size()) {
        return ""; // ถ้าผู้ใช้ไม่เลือกไฟล์หรือเลือกตัวเลือกที่ไม่ถูกต้อง
    }

    // ถ้าผู้ใช้เลือก "Back"
    if (choice == fileNames.size() - 1) {
        return ""; // การเลือก "Back" จะคืนค่าเป็นสตริงว่างเพื่อบ่งชี้ว่าต้องการกลับ
    }

    return jsonFiles[choice].string();
}


bool confirmDeletion(const string &filename) {
    vector<string> options = {"Yes", "No"};
    int choice = choiceSelection("Are you sure you want to delete " + filename + "?", options);

    return (choice == 0);
}

void QuizDeleting() {
    while (true) {
        switch (
            choiceSelection(
                "[Quiz Deleting] \n",
                {
                    "Delete a file",
                    "Back"
                }
            )
        ) {
        case 0: {
            fs::path directory = "D:\\Quiz\\QuizApp\\src\\data";
            string filepath = selectFileFromDirectory(directory);

            if (filepath.empty()) {
                // ถ้าผู้ใช้เลือก "Back" หรือไม่มีไฟล์ให้ลบ
                cout << "No file selected or going back to the previous menu." << endl;
                break;
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
            break;
        }
        case 1:
            return;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }
}
