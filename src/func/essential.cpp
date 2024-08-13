#include <iostream>
#include <vector>
#include <conio.h> // สำหรับ _getch()
#include <windows.h> // สำหรับ WinAPI

using namespace std;

#define UNDERLINE "\033[4m"
#define CLOSEUNDERLINE "\033[0m"
#define BOLD "\e[1m"
#define CLOSEBOLD "\e[0m"

void SetCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int choiceSelection(string title, const vector<string>& textChoice) {
    vector<string> options = textChoice;
    int choice = 0;
    int highlight = 0;

    while (true) {

        system("cls");

        cout << title << endl;

        for (int i = 0; i < options.size(); ++i) {
            if (i == highlight) {
                cout << BOLD << UNDERLINE << "> "; // แสดงเครื่องหมาย > สำหรับตัวเลือกที่ถูกไฮไลต์
            }
            else
                cout << "  "; // แสดงช่องว่างสำหรับตัวเลือกอื่น
                
            cout << options[i] << CLOSEUNDERLINE << CLOSEBOLD << endl;
        }

        cout << "\n Use \"arrow keys\" to select the options, Press \"Enter\" to select. \n\n";

        int c = _getch();

        if (c == 0 || c == 224) {
            c = _getch(); // อ่านค่าเพิ่มเติมสำหรับปุ่มลูกศร
        }

        switch (c) {
            case 72: // ลูกศรขึ้น
                highlight = (highlight == 0) ? options.size() - 1 : highlight - 1;
                break;
            case 80: // ลูกศรลง
                highlight = (highlight == options.size() - 1) ? 0 : highlight + 1;
                break;
            case 13: // ปุ่ม Enter
                choice = highlight;
                // cout << "You chose: " << choice << endl;
                return choice;
            default:
                break;
        }
    }
}
