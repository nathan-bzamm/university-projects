#include "Helper.h"
#include <iostream>

using std::cout;

string trimString(string s, const char delimeters[]){ // trim string of start and end delimeters
  bool cont = false;
  do{
    cont = false;
    for(unsigned int d = 0; d < strlen(delimeters); d++){
      if(s[0] == delimeters[d]){ // trim first letter each time if it matches the delimeter
        s.erase(0, 1);
        cont = true;
      }
    }
  } while (cont == true);

  do{
    cont = false;
    for(unsigned int d = 0; d < strlen(delimeters); d++){
      if(s[s.length()-1] == delimeters[d]){ // trim last letter if it matches the delimeter
        s.erase(s.length()-1, 1);
        cont = true;
      }
    }
  } while (cont == true);
  return s;
}

void viewStringAsASCII(string s){
  for(unsigned int i = 0; i < s.size(); ++i){
    cout << (int)s[i] << " ";
    if(s[i] == '\n'){
      cout << '\n';
    }
  }
}