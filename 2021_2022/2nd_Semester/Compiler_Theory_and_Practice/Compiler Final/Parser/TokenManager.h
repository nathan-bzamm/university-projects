#include <vector>
#include "../Lexer/Token.h"

using std::vector;

class TokenManager{
    private:
      //The index for the token queue
      unsigned int tokenIndex = 0;

      //Token queue
      vector<Token> *tokens;

    public:
      TokenManager(vector<Token> *_tokens) {tokens = _tokens; };
      Token *currentToken();
      Token *nextToken();
      Token *peekToken(int steps);
      Token *peekToken();
      Token *peekTokenUnsafe(int steps);
      int index() {return tokenIndex;}
};