#include "Parser.h"

Parser::Parser(vector<Token> *tokens){
  tokenManager = new TokenManager(tokens);
  tree = new ASTNodeProgram(tokenManager);
};

bool Parser::parse(){
  return tree->parse();
}

Parser::~Parser(){
  delete tree;
  delete tokenManager;
}