//
// Created by aidan on 19/04/2019.
//

#include <iostream>
#include "lexer.h"
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <tuple>


using namespace std;

const map<string, TOK > classificationTable = {
        {"float", TOK_TYPE}, {"int", TOK_TYPE}, {"bool", TOK_TYPE},
        {"true", TOK_BOOLEANLIT}, {"false", TOK_BOOLEANLIT},
        {"var", TOK_VARDECL},
        {"print", TOK_PRINT},
        {"return", TOK_RETURN},
        {"=", TOK_EQUALSSIGN}, {"_", TOK_UNDERSCORE},
        {"if", TOK_IF}, {"else", TOK_ELSE},
        {"for", TOK_FOR},
        {"fn", TOK_FUNCDECL},
        {"!", TOK_EXCLM},
        {"<", TOK_LT}, {">", TOK_GT}, {"==", TOK_EQ}, {"!=", TOK_NE}, {"<=", TOK_LE}, {">=", TOK_GE},
        {"+",TOK_ADD},{"-",TOK_MINUS},{"not",TOK_NOT},
        {"or",TOK_OR},{"*",TOK_MULT},{"/",TOK_DIV},{"and",TOK_AND},
        {"(", TOK_LBRACK},{")", TOK_RBRACK},{"{", TOK_BEGIN_SCOPE},{"}", TOK_END_SCOPE},{";", TOK_SEMICOLON},{":", TOK_COLON},{",", TOK_COMMA},
        {".", TOK_DOT},
        {"//", TOK_LINECOMMENT}, {"/*", TOK_BLOCKCOMMENT}, {"*\\", TOK_BLOCKCOMMENT},
        {" ", TOK_WHITESPACE},
        {"\n", TOK_ENDLINE}, {"\r",TOK_ENDLINE}
};

                           //START, ID_IN_PROGRESS, ID, TYPE, KEYWORD, INTEGER, ERROR, BAD
//const TOK TokenType[8] = {TOK_INVALID, TOK_ID, TOK_ID, TOK_TYPE, TOK_KEYWORD, TOK_INTEGERLIT, TOK_BOOLEANLIT, TOK_INVALID};

const TOK TokenType[BAD] = {TOK_INVALID, TOK_ID, TOK_INTEGERLIT, TOK_FLOATLIT,
                            TOK_TO_REPLACE, //equals
                            TOK_TO_REPLACE, //relop
                            TOK_TO_REPLACE, // punc
                            TOK_TO_REPLACE, // mult op
                            TOK_TO_REPLACE, // add op
                            TOK_TO_REPLACE,//exclm
                            TOK_TO_REPLACE, TOK_LINECOMMENT, TOK_BLOCKCOMMENT, TOK_TO_REPLACE,
                            TOK_INVALID, TOK_BLOCKCOMMENT};


State transitionTable[BAD][TOK_TABLE_SIZE] = {
//                 0    1        2         3        4      5       6       7       8       9     10    11   12
//                Lett  Dig      \n        =        _    WSpace    <       >       !       (      )    {    }
/*0 Start*/      {ID, INTEGER,  ERROR,   EQUALS,   ID, ERROR,  RELOP,  RELOP,  EXCLM,  PUNC,  PUNC, PUNC, PUNC,
//                 13      14       15      16     17               18          19       20
//                  :       ;        .       ,      *               /            +       -
                  PUNC,   PUNC,    PUNC,   PUNC, MULT_OP, COMMENT_IN_PROGRESS, ADD_OP, ADD_OP},

//                 0    1      2     3     4    5       6       7       8       9     10      11   12
//                Lett  Dig   \n     =     _   WSpace    <       >       !       (      )      {    }
/*1 ID*/         {ID,   ID,  ERROR, ERROR, ID, ERROR,  ERROR,  ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR,
//                 13      14       15      16     17       18      19       20
//                  :       ;        .       ,      *       /        +       -
                  ERROR,   ERROR,  ERROR,  ERROR, ERROR, ERROR,   ERROR, ERROR},

//                 0     1      2     3     4        5       6       7       8       9     10      11   12
//                Lett   Dig   \n     =     _      WSpace    <       >       !       (      )      {    }
/*2 INTEGER*/    {ERROR, INTEGER,  ERROR, ERROR, ERROR, ERROR,  ERROR,  ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR,
//                 13      14       15      16     17       18      19     20
//                  :       ;        .       ,      *       /        +     -
                  ERROR,   ERROR,  FLOAT,  ERROR, ERROR, ERROR,   ERROR, ERROR},

//                 0     1      2     3     4        5       6       7       8       9     10      11   12
//                Lett   Dig   \n     =     _      WSpace    <       >       !       (      )      {    }
/*3 FLOAT*/      {ERROR, FLOAT,  ERROR, ERROR, ERROR, ERROR,  ERROR,  ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR,
//                 13      14       15      16     17       18      19     20
//                  :       ;        .       ,      *       /        +     -
                  ERROR,   ERROR,  ERROR,  ERROR, ERROR, ERROR,   ERROR, ERROR},

//                 0     1        2     3       4     5       6       7       8       9     10      11   12
//                Lett   Dig     \n     =       _    WSpace    <       >       !       (      )      {    }
/*4 EQUALS*/     {ERROR, ERROR, ERROR, RELOP, ERROR, ERROR,  ERROR,  ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR,
//                 13      14       15      16     17       18      19     20
//                  :       ;        .       ,      *       /        +     -
                  ERROR,   ERROR,  ERROR,  ERROR, ERROR, ERROR,   ERROR, ERROR},

//                 0      1       2     3      4     5       6       7       8       9     10      11   12
//                Lett   Dig     \n     =      _    WSpace    <       >       !       (      )      {    }
/*5 RELOP*/     {ERROR, ERROR, ERROR, RELOP, ERROR, ERROR,  ERROR,  ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR,
//                 13      14       15      16     17       18   19     20
//                  :       ;        .       ,      *       /     +      -
                  ERROR,   ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR, ERROR},

//                 0      1       2     3      4     5       6       7       8       9     10      11   12
//                Lett   Dig     \n     =      _    WSpace    <       >       !       (      )      {    }
/*6 PUNC*/      {ERROR, ERROR, ERROR, ERROR, ERROR, ERROR,  ERROR,  ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR,
//                 13      14       15      16     17       18   19     20
//                  :       ;        .       ,      *       /     +      -
                  ERROR,   ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR, ERROR},

//                 0      1       2     3      4     5       6       7       8       9     10      11   12
//                Lett   Dig     \n     =      _    WSpace    <       >       !       (      )      {    }
/*7 MULT_OP*/   {ERROR, ERROR, ERROR, ERROR, ERROR, ERROR,  ERROR,  ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR,
//                 13      14       15      16     17       18   19     20
//                  :       ;        .       ,      *       /     +      -
                  ERROR,   ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR, ERROR},

//                 0      1       2     3      4     5       6       7       8       9     10      11   12
//                Lett   Dig     \n     =      _    WSpace    <       >       !       (      )      {    }
/*8 ADD_OP*/    {ERROR, ERROR, ERROR, ERROR, ERROR, ERROR,  ERROR,  ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR,
//                 13      14       15      16     17       18   19     20
//                  :       ;        .       ,      *       /     +      -
                  ERROR,   ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR, ERROR},

//                 0      1       2     3      4     5       6       7       8       9     10      11   12
//                Lett   Dig     \n     =      _    WSpace    <       >       !       (      )      {    }
/*9 EXCLM*/     {ERROR, ERROR, ERROR, RELOP, ERROR, ERROR,  ERROR,  ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR,
//                 13      14       15     16     17       18   19     20
//                  :       ;        .      ,      *       /     +      -
                  ERROR,  ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR, ERROR},

//                   0         1        2        3        4       5          6         7         8       9         10       11       12
//                  Lett      Dig      \n        =        _      WSpace      <         >         !       (          )       {        }
/*10 CMT_IN_PROG*/{ERROR, ERROR, ERROR, ERROR, ERROR, ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR, ERROR, ERROR,
//                 13       14     15     16     17          18       19          20
//                  :       ;       .      ,      *           /        +          -
                  ERROR, ERROR,  ERROR,  ERROR, B_COMMENT, L_COMMENT, ERROR, ERROR},

//                   0         1        2        3             4       5             6         7            8        9         10       11         12
//                  Lett      Dig      \n        =             _      WSpace         <         >            !        (          )         {          }
/*11 L_COMMENT*/{L_COMMENT, L_COMMENT, ERROR, L_COMMENT, L_COMMENT, L_COMMENT,  L_COMMENT,  L_COMMENT, L_COMMENT, L_COMMENT, L_COMMENT, L_COMMENT, L_COMMENT,
//                    13          14         15          16          17          18        19          20
//                     :          ;          .            ,           *           /         +          -
                  L_COMMENT,  L_COMMENT,  L_COMMENT,  L_COMMENT, L_COMMENT, L_COMMENT, L_COMMENT, L_COMMENT},

//                   0         1        2         3            4        5            6         7            8        9         10       11         12
//                  Lett      Dig      \n         =            _      WSpace         <         >            !        (          )           {          }
/*12 B_COMMENT*/{B_COMMENT, B_COMMENT, B_COMMENT, B_COMMENT, B_COMMENT, B_COMMENT,  B_COMMENT,  B_COMMENT, B_COMMENT, B_COMMENT, B_COMMENT, B_COMMENT, B_COMMENT,
//                    13          14         15          16          17          18        19          20
//                     :          ;          .            ,           *           /         +          -
                  B_COMMENT,  B_COMMENT,  B_COMMENT, B_COMMENT, END_B_COMMENT, B_COMMENT, B_COMMENT, B_COMMENT},

//                       0         1        2         3            4        5            6         7            8        9         10       11         12
//                       Lett      Dig      \n         =            _      WSpace         <         >            !        (          )           {          }
/*13 END_B_COMMENT*/ {B_COMMENT, B_COMMENT, B_COMMENT, B_COMMENT, B_COMMENT, B_COMMENT,  B_COMMENT,  B_COMMENT, B_COMMENT, B_COMMENT, B_COMMENT, B_COMMENT, B_COMMENT,
//                    13          14         15          16          17          18        19          20
//                     :          ;          .            ,           *           /         +          -
                  B_COMMENT,  B_COMMENT,  B_COMMENT, B_COMMENT, END_B_COMMENT, FIN_B_COM, B_COMMENT, B_COMMENT},


//                 0      1       2     3      4     5       6       7       8       9     10      11   12
//                Lett   Dig     \n     =      _    WSpace    <       >       !       (      )      {    }
/*14 ERROR*/    {ERROR, ERROR, ERROR, ERROR, ERROR, ERROR,  ERROR,  ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR,
//                 13      14       15      16     17       18   19     20
//                  :       ;        .       ,      *       /     +      -
                 ERROR,   ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR, ERROR},

        //                0      1       2     3      4     5       6       7       8       9     10      11   12
//                      Lett   Dig     \n     =      _    WSpace    <       >       !       (      )      {    }
/*15 FIN_B_COM*/     {ERROR, ERROR, ERROR, RELOP, ERROR, ERROR,  ERROR,  ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR,
//                           13      14       15     16     17       18   19     20
//                           :       ;        .      ,      *       /     +      -
                         ERROR,  ERROR,  ERROR,  ERROR, ERROR, ERROR, ERROR, ERROR},
};

Lexer::Lexer(const string& parseString){
    stringToParse = parseString;
    tokenVector = vector<Token*>();
    charIndex = 0;
    lineNumber = 1;
    this->parseString();
}

Lexer::~Lexer() = default;

Token* Lexer::nextWord(){
    bool increaseLineNumber = false;
    vector<State> stateStack(0);
    string lexeme ="";
    State currentState = START;
    stateStack.clear();
    stateStack.push_back(BAD);

    while(currentState!=ERROR){
        char character;
        nextChar(&character);
        lexeme += character;
        //cout << "TOKEN TYPE:" << TokenType[currentState] <<endl;
        if(TokenType[currentState] != TOK_INVALID)
            stateStack.clear();
        stateStack.push_back(currentState);
        TOK cat = charCategory(character);
        if(cat != TOK_LEXERROR) {
            currentState = transitionTable[currentState][cat];
            if(currentState == FIN_B_COM) return createToken(TOK_BLOCKCOMMENT,lexeme); //quick hack since block comments weren't working
            if (currentState == ERROR && cat == TOK_ENDLINE && character == '\n') // doing this because of \r
                increaseLineNumber = true;
            if (currentState == ERROR && !isTokenToDiscard(cat)) // this is because when encountering a */, the / was being seen as an error
                charIndex--; //this is to not discard characters which are useful.
        }
        else
            reportLexerError(character);
    }

    //Roll back loop
    while(TokenType[currentState] == TOK_INVALID && currentState != BAD){
        currentState = stateStack.back();
        stateStack.pop_back();
        if(lexeme.length() > 0)
            lexeme = lexeme.substr(0, lexeme.length() - 1); // truncating lexeme
    }

    TOK finalToken = TOK_INVALID;
    if(lexeme.length() > 0)
        finalToken = idToKeyword(&lexeme, TokenType[currentState]);
    if(finalToken == TOK_TO_REPLACE){   //it may be a relop. Figure out what type it is or return error
        auto tableResult = classificationTable.find(lexeme);
        while(tableResult == classificationTable.end()){ //while not found
            lexeme = lexeme.substr(0, lexeme.length() - 1); //truncate lexeme until matching relop is found
            charIndex--; // roll back index to prevent discarding other operators
            tableResult = classificationTable.find(lexeme);
        }
    }
    Token* returnToken = createToken(finalToken,lexeme);
    if (increaseLineNumber)lineNumber++;
    return returnToken;
}

TOK Lexer::idToKeyword(const string* identifier, TOK originalToken){
    auto tableResult = classificationTable.find(*identifier);
    if (tableResult != classificationTable.end()){ //found
        return tableResult->second; //return token type
    }
    else return originalToken;
}

TOK Lexer::charCategory(char character){
    //using ASCII ranges to check for digit or letter
    if(character >= 48 && character <= 57) return TOK_DIGIT;
    else if((character >= 65 && character <= 90) || (character >= 97 && character <= 122)) return TOK_LETTER;
    if(character == '\000')return TOK_ENDLINE; //since parseString() adds an eof after finishing lexing run, we can discard this
    string s(1,character); // Converting character to string for table
    auto tableResult = classificationTable.find(s);
    if (tableResult != classificationTable.end()){ //found
        return tableResult->second; //return token type
    }
    return TOK_LEXERROR; // if no token is found up to this point, token doesnt exist
}

//! Creates a token, but does some conversions if necessary
Token* Lexer::createToken(TOK type, std::string value){
    if(type==TOK_FLOATLIT||type==TOK_INTEGERLIT){
        return new Token(type,stof(value),lineNumber);
    }
    else if (type==TOK_BOOLEANLIT){
        return new Token(type,(bool) (value == "true"),lineNumber);
    }
    else return new Token(type,value,lineNumber);
}

void Lexer::nextChar(char* character){
    if(charIndex <= stringToParse.length()) {
        *character = stringToParse[charIndex];
        charIndex++;
    }
}

void Lexer::parseString(){
    while(charIndex <stringToParse.length()){
        Token* nextToken = nextWord();
        if(nextToken->tokenType == TOK_INVALID || isTokenToDiscard(nextToken->tokenType)){
            delete nextToken;
        }
        else
            tokenVector.push_back(nextToken);
    }
    tokenVector.push_back(new Token(TOK_EOF,"",lineNumber)); // to signal no more tokens
    cout<<"FINISHED LEXING"<<endl;
    cout<<"LIST OF TOKENS"<<endl;
    for(auto i : tokenVector) {
        try {
            if (i->name != "")
                cout << "(" << tokenToString.at(i->tokenType) << ", \'" << i->name << "\', " << i->lineNumber << " )"
                     << endl;
            else
                cout << "(" << tokenToString.at(i->tokenType) << ", \'" << i->value << "\', " << i->lineNumber << " )"
                     << endl;
        }catch(const out_of_range& err){
            cerr<<"Invalid token in line "<<i->lineNumber<<": "<<i->name<<endl; //not the best error handling but screw it
            exit(EXIT_FAILURE);
        }
    }
}

bool Lexer::isTokenToDiscard(TOK token){
    return ( token == TOK_LINECOMMENT || token == TOK_BLOCKCOMMENT||
             token == TOK_ENDLINE || token == TOK_WHITESPACE);
}

Token* Lexer::getNextToken(){
    auto nextToken = tokenVector.front();
    tokenVector.erase(tokenVector.begin());
    return nextToken;
}

/* Does not consume the token */
Token* Lexer::peekNextToken(){
    return tokenVector.front();
}

void Lexer::reportLexerError(char character) {
    cerr<<"Invalid character in line "<<lineNumber<<": "<< character<<endl;
    exit(EXIT_FAILURE);
}
