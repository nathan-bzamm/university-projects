#ifndef Helper_H
#define HELPER_H

#include <string>
#include <string.h>
#include <vector>

using std::string;
using std::vector;

// a method to get size of an array
// template <typename T,unsigned S>
// inline unsigned arraysize(const T (&v)[S]) { return S; }

// method to be called at exit (example: free up pointers)
void cleanup(); 

// trim string of start and end delimeters
string trimString(string s, const char delimeters[]); 

// for debugging purposes. given a string, prints it's ASCII integer values
void viewStringAsASCII(string s); 

#endif // HELPER_H