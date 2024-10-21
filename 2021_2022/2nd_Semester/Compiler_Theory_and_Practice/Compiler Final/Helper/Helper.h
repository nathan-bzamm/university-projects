#ifndef Helper_H
#define HELPER_H

#include <string>
#include <string.h>
#include <vector>

using std::string;
using std::vector;

//method to be called at exit
//Example: Free up pointers
void cleanup();

//Trim string of start and end delimeters
string trimString(string s, const char delimeters[]);

//Used to debug
//Will print a given string's ASCII integer values
void viewStringAsASCII(string s);

#endif //HELPER_H