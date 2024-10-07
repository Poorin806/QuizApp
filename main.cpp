#include <iostream>
#include <conio.h>
#include <string>

#include "src/header/essential.h"
#include "src/header/groupFunc.h"

using namespace std;

void quizManagement();

int main() {

    // Main menu    
    while (true) {

        switch (
            choiceSelection(
                "[Quiz Taker & Maker] \n",
                {
                    "Take a test",
                    "Quiz Management",
                    "Log (History)",
                    "About",
                    "Exit"
                }
            )
        ) {
            case 0: // Take a test
                QuizTaking();
                break;
            case 1: // Quiz Management
                quizManagement();
                break;
            case 2: // Quiz Log (History)
                QuizLog();
                break;
            case 3: // About
                system("cls");
                cout << "[Quiz Taker & Maker]" << endl << endl;
                
                // About this app
                cout << "* About this app:" << endl;
                cout << "- Quiz App: V.1.0 (Full Release)" << endl;

                cout << endl;

                cout << "* Developed by: " << endl;
                cout << "- Mr.Poorin Thatsakhon \t\tITI. (Sec.B) - 6706021411202" << endl;
                cout << "- Mr.Theerapat Phetchinda \tITI. (Sec.B) - 6706021411172" << endl;
                cout << "- Mr.Krittipong kumpluem \tITI. (Sec.B) - 6706021411024" << endl;
                cout << "- Mr.Nontapat Ngajuer \t\tITI. (Sec.B) - 6706021411296" << endl;

                cout << endl;

                cout << "Press any key to continue...";

                _getch();
                break;
            case 4:
                cout << "Exiting..." << endl;
                return 0;
        }
    }

    return 0;

}

void quizManagement() {

    while (true) {

        switch (
            choiceSelection(
                "[Quiz Management] \n",
                {
                    "Create a quiz",
                    "Edit a quiz",
                    "Delete a quiz",
                    "Back"
                }
            )
        ) {
            case 0:
                QuizCreating();
                break;
            case 1:
                QuizEditing();
                break;
            case 2:
                QuizDeleting();
                break;
            case 3:
                return;
        }
    }

}