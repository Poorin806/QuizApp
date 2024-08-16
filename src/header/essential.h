#ifndef ESSENTIAL_H
#define ESSENTIAL_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// [Essentials Modules]
int choiceSelection(string title, const vector<string>& textChoice);
vector<string> listJsonFile();
string generateUUID();

#endif