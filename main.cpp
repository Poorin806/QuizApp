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
                "[Quiz Maker & Taker] \n",
                {
                    "Take a test",
                    "Quiz Management",
                    "About",
                    "Exit"
                }
            )
        ) {
            case 0:
                QuizTaking();
                break;
            case 1:
                quizManagement();
                break;
            case 2:
                system("cls");
                cout << "About" << endl;
                cout << "Press any key to continue..." << endl;
                _getch();
                break;
            case 3:
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