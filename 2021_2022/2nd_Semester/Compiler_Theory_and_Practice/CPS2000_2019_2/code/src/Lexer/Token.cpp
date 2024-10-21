#include <stdlib.h>
#include <iostream>
#include "Token.h"
#include "../Helper/helper.h"

using std::cout;

string Token::TokenString[] = {
  "ID", "BOOL", "FLOAT", "INT", // ID and constants
  "ST", "SE", "GT", "GE", "EQQ", "NE", "AND", "OR", "NOT", // conditional operators
  "EQ", "PLUS", "MINUS", "TIMES", "DIVISION", //arithmetic operators
  "IF", "ELSE", "FOR", "WHILE", "FN", "RETURN", "TYPE_BOOL", "TYPE_FLOAT", "TYPE_INT", "VAR", //keywords
  "COLON", "SEMI_COLON", "COMMA", //punctuation
  "OPEN_BRACKET", "CLOSED_BRACKET", "OPEN_BRACE", "CLOSED_BRACE", //brackets punctuation
  "COMMENT",
  "PRINT" // print keyword
};

void Token::printToken(){
  cout << "<" << lexeme << ", " << Token::TokenString[type] << ", " << number << ">\n";
}

// Used to check if a given string is a keyword (or identifier), and what type of keyword it is
struct keyword_token{
  string text;
  TokenType tok_type;
};

keyword_token my_keywords[] = {
  {"and", AND},
  {"or", OR},
  {"not", NOT},
  {"if", IF},
  {"else", ELSE},
  {"for", FOR},
  {"while", WHILE},
  {"fn", FN},
  {"return", RETURN},
  {"bool", TYPE_BOOL},
  {"float", TYPE_FLOAT},
  {"int", TYPE_INT},
  {"var", VAR},
  {"true", BOOL},
  {"false", BOOL},
  {"print", PRINT}
};

Token processAlpha(string lexeme, int lineNumber){
  for(uint8_t i = 0; i < sizeof(my_keywords)/sizeof(keyword_token); ++i){
    if(lexeme == my_keywords[i].text){
      if(lexeme == "true") return Token(my_keywords[i].tok_type, lexeme, 1, lineNumber);
      return Token(my_keywords[i].tok_type, lexeme, 0, lineNumber);
    }
  }
  return Token(ID, lexeme, 0, lineNumber);
}

//struct used to assign tokens to normal punctuation
struct character_token{
  char character;
  TokenType token;
};

character_token normal_punctuation[] = {
  {'+', PLUS},
  {'-', MINUS},
  {'(', OPEN_BRACKET},
  {')', CLOSED_BRACKET},
  {'{', OPEN_BRACE},
  {'}', CLOSED_BRACE},
  {':', COLON},
  {';', SEMI_COLON},
  {',', COMMA},
  {'<', ST},
  {'>', GT},
  {'*', TIMES}
};

Token *processNormalPunctuation(string lexeme, int lineNumber){
  Token *t = nullptr;
  for(unsigned int i = 0; i < sizeof(normal_punctuation)/sizeof(character_token); ++i){
    if(lexeme[0] == normal_punctuation[i].character){
      t = new Token(normal_punctuation[i].token, lexeme, 0.0f, lineNumber);
    }
  }
  return t;
}