#include "TokenManager.h"
#include <iostream>

Token *TokenManager::nextToken(){
  if(tokenIndex < tokens->size()){
    Token *ret = &tokens->at(tokenIndex);
    tokenIndex++;
    return ret;
  }
  return nullptr;
}

Token *TokenManager::peekToken(int steps){
  if(tokenIndex+steps < tokens->size()){
    return &tokens->at(tokenIndex + steps);
  }
  std::cerr << "Reached end of file while parsing\n";
  exit(EXIT_SUCCESS);
}

Token *TokenManager::peekToken(){
  return peekToken(0);
}

Token *TokenManager::peekTokenUnsafe(int steps){
  if(tokenIndex+steps < tokens->size()){
    return &tokens->at(tokenIndex + steps);
  }
  return nullptr;
}