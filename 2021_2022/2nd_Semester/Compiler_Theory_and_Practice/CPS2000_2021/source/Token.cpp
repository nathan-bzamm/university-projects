//
// Created by aidan on 30/04/2019.
//

#include <unordered_map>
#include "Token.h"

using namespace std;

Token::Token(TOK type,const string name, int lineNumber){
    tokenType = type;
    this->name = name;
    this->lineNumber = lineNumber;
}

Token::Token(TOK type,float value,int lineNumber){
    tokenType = type;
    this->value = value;
    this->lineNumber = lineNumber;
}

Token::Token(TOK type,bool value,int lineNumber){
    tokenType = type;
    this->value = value;
    this->lineNumber = lineNumber;
}

Token::~Token() = default;