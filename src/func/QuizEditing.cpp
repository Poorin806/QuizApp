// Quiz Editing (Poorin's Part)

#include <iostream>
#include <vector>
#include <conio.h> // สำหรับ _getch()

#include <windows.h>
#include <string.h>

#include "../header/essential.h"

using namespace std;

void QuizEditing() {

    while (true) {

        switch (
            choiceSelection(
                "[Quiz Editing] \n",
                {
                    "1",
                    "2",
                    "3",
                    "Exit"
                }
            )
        ) {
            case 0:
            case 1:
            case 2:
                break;
            case 3:
                return;
        }

    }

    _getch();
    cout << "Press any key to continue..." << endl;

    return;

}