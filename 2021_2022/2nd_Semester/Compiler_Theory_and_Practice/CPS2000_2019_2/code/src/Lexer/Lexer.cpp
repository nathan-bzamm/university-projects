#include <iostream>
#include "Lexer.h"
#include "../Helper/Helper.h"
#include "string.h"

using std::cerr;

void Lexer::lex(){
  unsigned int fileIndex = 0; // index in the file. Increased by 1 with each character read
  char c; // the current character being read
  State state = STA; // the current state. Starts at the start state
  string lexeme = ""; // the current lexeme. Starts off empty

  do{ // read character by character until end of file
    c = file[fileIndex++]; // get next character

    if(c == '\n'){
      lineNumber++; // for debugging
    }

    State newState = transitionTable[state] [getColumn(c)];
    
    if(newState == ERR){ // if at an error state (end of lexeme)
      process_lexeme(lexeme, state); // adds lexeme as a token to the tokens vector
      
      // reduce file index by 1 to reread current character since it lead to an error state
      fileIndex--; 

      // reduce line number or you get \n read twice
      if(c == '\n' && state){
        lineNumber--;
      }
      
      // reset for new lexeme
      state = STA;
      lexeme = "";
    }else{ // if lexeme is still not over
      lexeme += c; // append c to lexeme
      state = newState;
    }
  }while(fileIndex < file.size());
}

int Lexer::getColumn(char c){
  switch (c){
    case '+':
    case '-':
    case '(':
    case ')':
    case '{':
    case '}':
    case ';':
    case ':':
    case ',':
      return 0;
    break;
    
    case '!':
      return 1;
    break;

    case '.':
      return 2;
    break;

    case '/':
      return 5;
    break;

    case '\n':
      return 6;
    break;

    case '*':
      return 7;
    break;

    case '=':
      return 8;
    break;

    case '\r':
    case ' ':
    case '\t':
      return 9;
    break;

    case '<':
    case '>':
      return 10;
    break;

    default:
      if(isdigit(c)){
        return 3;
      }else if(isalpha(c) || c == '_'){
        return 4;
      }else{
        return 11;
      }
  }
}

void Lexer::process_lexeme(string lexeme, State state){
  // needed in cases S10 and S12 (for comments)
  string temp;
  int lexeme_length;
  char delimeters[] = {' ', '\t', '\n', '\r'}; 

  switch(state){
    case S00: // +-(){};:
      tokens.push_back(*processNormalPunctuation(lexeme, lineNumber));
    break;

    case S02: // !=
      tokens.push_back(Token(NE, lexeme, 0.0f, lineNumber));
    break;
    
    case S03: // integer
      tokens.push_back(Token(INT, lexeme, stoi(lexeme), lineNumber));
    break;
    
    case S05: // float
      tokens.push_back(Token(FLOAT, lexeme, stof(lexeme), lineNumber));
    break;
    
    case S06: // random string (keyword or alpha)
      tokens.push_back(processAlpha(lexeme, lineNumber));
    break;
    
    case S07: // /
      tokens.push_back(Token(DIVISION, lexeme, 0.0f, lineNumber));
    break;
    
    case S10: // multi line comment
      lexeme_length = lexeme.length();
      temp.assign(lexeme, 2, lexeme_length-4); // trim '/*' and '*/'
      tokens.push_back(Token(COMMENT, trimString(temp, delimeters), 0.0f, lineNumber));
    break;
 
    case S12: // single line comment
      lexeme_length = lexeme.length();
      temp.assign(lexeme, 2, lexeme_length-2); // trim '//'
      tokens.push_back(Token(COMMENT, trimString(temp, delimeters), 0.0f, lineNumber));
    break;

    case S13: // =
      tokens.push_back(Token(EQ, lexeme, 0.0f, lineNumber));
    break;

    case S14: // ==
      tokens.push_back(Token(EQQ, lexeme, 0.0f, lineNumber));
    break;

    case S15: // <>
      tokens.push_back(*processNormalPunctuation(lexeme, lineNumber));
    break;

    case S16: // <= >=
      if(lexeme == "<="){
        tokens.push_back(Token(SE, lexeme, 0.0f, lineNumber));
      }else{
        tokens.push_back(Token(GE, lexeme, 0.0f, lineNumber));
      }
    break;

    case S17: // empty characters
      // do nothing
    break;

    default:
      cerr << "Lexing error: Invalid lexeme " << lexeme.size() << " at line " << lineNumber << "\n";
      exit(EXIT_FAILURE);
    break;
  }
}

void Lexer::printTokens(){
  for(unsigned int i = 0; i < tokens.size(); ++i){
    tokens[i].printToken();
  }
}

void Lexer::removeComments(){
  for(unsigned int i = 0 ; i < tokens.size(); ++i){
    if(tokens[i].type == COMMENT){
      tokens.erase(tokens.begin() + i);
      i--;
    }
  }
}