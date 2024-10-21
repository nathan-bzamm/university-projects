#include <vector>
#include "Token.h"

using std::vector;

class Lexer {
    private:
      int lineNumber = 1; //used to debug

      string file; //the string which will be lexed

      //The following is a list of states
      //STA being the start state
      //ERR being the error state
      enum State{
      STA, S00, S01, S02, S03, S04, S05, S06, S07, S08, S09, S10, S11, S12, S13, S14, S15, S16, S17, ERR
      };
      //
      State transitionTable[19][12] = {
        /*         0         1    2    3      4      5    6    7    8    9          10   11
                   +-(){};:, !    .    digit  alpha  /    \n  *    =    space/tab\r <>   other*/
        /*00 STA*/ {S00,     S01, S04, S03,   S06,   S07, S17, S00, S13, S17,       S15, ERR},
        /*01 S00*/ {ERR,     ERR, ERR, ERR,   ERR,   ERR, ERR, ERR, ERR, ERR,       ERR, ERR},
        /*02 S01*/ {ERR,     ERR, ERR, ERR,   ERR,   ERR, ERR, ERR, S02, ERR,       ERR, ERR},
        /*03 S02*/ {ERR,     ERR, ERR, ERR,   ERR,   ERR, ERR, ERR, ERR, ERR,       ERR, ERR},
        /*04 S03*/ {ERR,     ERR, S04, S03,   ERR,   ERR, ERR, ERR, ERR, ERR,       ERR, ERR},
        /*05 S04*/ {ERR,     ERR, ERR, S05,   ERR,   ERR, ERR, ERR, ERR, ERR,       ERR, ERR},
        /*06 S05*/ {ERR,     ERR, ERR, S05,   ERR,   ERR, ERR, ERR, ERR, ERR,       ERR, ERR},
        /*07 S06*/ {ERR,     ERR, ERR, S06,   S06,   ERR, ERR, ERR, ERR, ERR,       ERR, ERR},
        /*08 S07*/ {ERR,     ERR, ERR, ERR,   ERR,   S11, ERR, S08, ERR, ERR,       ERR, ERR},
        /*09 S08*/ {S08,     S08, S08, S08,   S08,   S08, S08, S09, S08, S08,       S08, S08},
        /*10 S09*/ {S08,     S08, S08, S08,   S08,   S10, S08, S08, S08, S08,       S08, S08},
        /*11 S10*/ {ERR,     ERR, ERR, ERR,   ERR,   ERR, ERR, ERR, ERR, ERR,       ERR, ERR},
        /*12 S11*/ {S11,     S11, S11, S11,   S11,   S11, S12, S11, S11, S11,       S11, S11},
        /*13 S12*/ {ERR,     ERR, ERR, ERR,   ERR,   ERR, ERR, ERR, ERR, ERR,       ERR, ERR},
        /*14 S13*/ {ERR,     ERR, ERR, ERR,   ERR,   ERR, ERR, ERR, S14, ERR,       ERR, ERR},
        /*15 S14*/ {ERR,     ERR, ERR, ERR,   ERR,   ERR, ERR, ERR, ERR, ERR,       ERR, ERR},
        /*16 S15*/ {ERR,     ERR, ERR, ERR,   ERR,   ERR, ERR, ERR, S16, ERR,       ERR, ERR},
        /*17 S16*/ {ERR,     ERR, ERR, ERR,   ERR,   ERR, ERR, ERR, ERR, ERR,       ERR, ERR},
        /*18 S17*/ {ERR,     ERR, ERR, ERR,   ERR,   ERR, S17, ERR, ERR, S17,       ERR, ERR} 
      };
      int getColumn(char c);//checking which column the character belongs to

      //check which token the lexeme belongs to
      //add it to the list of vectors
      //or report the error
      void processLexeme(string lexeme, State state);

      //the list of tokens
      vector<Token> tokens;
    public:
      Lexer(string filestring){file = filestring; }
      vector<Token> get_tokens(){return tokens; }//returning the list of tokens
      void printTokens(); //This is a Helper function to print all the tokens in a neat manner

      void lex(); //main method to obtain all tokens from the file
      void removeComments(); //will remove the comments from the tokens

};