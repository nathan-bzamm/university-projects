#include <vector>
#include "../Lexer/Token.h"

using std::vector;

class TokenManager{
  private:
    unsigned int tokenIndex = 0; // The index, for the token queue
    vector<Token> *tokens; // Token queue
  public:
    TokenManager(vector<Token> *_tokens){ tokens = _tokens; };
	  Token *currentToken();
    Token *nextToken(); // Dequeue and return next token
    Token *peekToken(int steps); // Peek more than 1 token away
    Token *peekToken(); // See what the next token is without dequeuing it
    Token *peekTokenUnsafe(int steps); // Returns null ptr rather thn exiting if there is no token
    int index(){ return tokenIndex; }
};
