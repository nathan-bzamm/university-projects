#include <vector>
#include <string>
#include "Token.h"
#pragma once

enum State{
    //KEYWORD,//VAR, IF, FOR, FN, RETURN, PRINT,/*TYPE,*/
    START, ID,  INTEGER, FLOAT,
    EQUALS,
    RELOP, //LT, GT, EQ,LE,GE,NE,
    PUNC, //LBRACE, RBRACE, LBRACK, RBRACK, DOT, COLON, SEMICOL, COMMA,
    MULT_OP, //MULT, DIV, AND,
    ADD_OP, //PLUS, MINUS, OR,
    EXCLM,
    COMMENT_IN_PROGRESS, L_COMMENT, B_COMMENT,END_B_COMMENT, ERROR, FIN_B_COM,BAD
};

class Lexer{
private:
    int charIndex;
    int lineNumber;
    std::vector<Token*> tokenVector;
    std::string stringToParse;
    Token* nextWord();
    void nextChar(char* character);
    TOK charCategory(char character);
    TOK idToKeyword(const std::string* identifier, TOK originalToken);
    bool isTokenToDiscard(TOK token);
    void parseString();
    Token* createToken(TOK type, std::string value);
    void reportLexerError(char character);

public:
    Lexer(const std::string& parseString);
    ~Lexer();
    Token* getNextToken();
    Token* peekNextToken();
};