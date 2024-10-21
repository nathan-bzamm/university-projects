#include <iostream>

#include "Visitor.h"
#include "../Helper/Helper.h"

using std::map;
using std::cerr;
using std::cout;

void SAVisitor::newScope(){
  scope.insert(scope.begin() ,map<string, TokenType>());
}
void SAVisitor::insert(string s, TokenType tt){
  // if lexeme already exists in current scope or is a function name
  if(scope[0].count(s) || functions.count(s)){
    cerr << "Symbol " << s << " at line " << lineNumber;
    cerr << " already exists\n";
    exit(EXIT_FAILURE);
  }
  scope[0][s] = tt;
}
void SAVisitor::removeScope(){
  scope.erase(scope.begin());
}
TokenType* SAVisitor::lookup(string s){
  for(unsigned int i = 0; i < scope.size(); ++i){
    if(scope[i].count(s)){ // if key exists
      return &scope[i].find(s)->second;
    }
  }
  return nullptr;
}
void SAVisitor::printSymbolTable(){
  cout << "Symbol Table:\n";
  for(unsigned int i = 0; i < scope.size(); ++i){
    cout << "scope " << i << std::endl;
    map<string, TokenType>::iterator it;
    for ( it = scope[i].begin(); it != scope[i].end(); it++ ){
      cout << it->first << " : " << Token::TokenString[it->second] << std::endl;
    }
    cout << "\n\n";
  }
  cout << "Functions Table:\n";
  map<string, vector<TokenType>>::iterator it;
  for ( it = functions.begin(); it != functions.end(); it++ ){
    cout << it->first << " : ";
    for(unsigned int i = 0; i < it->second.size(); ++i){
      cout << Token::TokenString[it->second[i]] << ", ";
    }
    cout << "\n";
  }
  cout << "\n";
}


void *SAVisitor::visit(ASTNodeType *n){
  TokenType *tt = nullptr;
  switch(n->token->type){
    case TYPE_FLOAT:
      tt = new TokenType(FLOAT);
    break;
    case TYPE_INT:
      tt = new TokenType(INT);
    break;
    case TYPE_BOOL:
      tt = new TokenType(BOOL);
    break;
    default:
    break;
  }
  lineNumber = n->token->lineNumber;
  return (TokenType*)tt;
}
void *SAVisitor::visit(ASTNodeLiteral *n){
  lineNumber = n->token->lineNumber;
  return (TokenType*)&n->token->type;
}
void *SAVisitor::visit(ASTNodeIdentifier *n){
  lineNumber = n->token->lineNumber;
  return (string*)(&n->token->lexeme);
}
void *SAVisitor::visit(ASTNodeMultiplicativeOp *n){
  lineNumber = n->token->lineNumber;
  return (TokenType*)&n->token->type;
}
void *SAVisitor::visit(ASTNodeAdditiveOp *n){
  lineNumber = n->token->lineNumber;
  return (TokenType*)&n->token->type;
}
void *SAVisitor::visit(ASTNodeRelationalOp *n){
  lineNumber = n->token->lineNumber;
  return (TokenType*)&n->token->type;
}
void *SAVisitor::visit(ASTNodeActualParams *n){
  vector<TokenType*> *types = new vector<TokenType*>();
  for(unsigned int i = 0; i < n->expressions.size(); ++i){
    types->push_back((TokenType*)n->expressions[i]->accept(this));
  }
  return (vector<TokenType*>*)types;
}
void *SAVisitor::visit(ASTNodeFunctionCall *n){
  // if function not found in functions table
  string *fname = (string*)n->identifier->accept(this);
  if(! functions.count( *fname )){
    cerr << *fname << " at line number ";
    cerr << lineNumber << " is not a function." << "\n";
    exit(EXIT_FAILURE);
  }
  // check parmaters are correct
  // check size
  vector<TokenType> reqd = functions.find(*fname)->second;
  if(n->actualParams == NULL && reqd.size() > 0){
    cerr << "Invalid parameters for function " << *fname;
    cerr << " at line number " << lineNumber << "\n";
    exit(EXIT_FAILURE);
  }
  if(reqd.size() == 0 && !n->actualParams) return (TokenType*)lookup(*fname);
  vector<TokenType*> vtt = *(vector<TokenType*>*)n->actualParams->accept(this);
  if(vtt.size() != reqd.size()){
    cerr << "Invalid parameters for function " << *fname;
    cerr << " at line number " << lineNumber << "\n";
    exit(EXIT_FAILURE);
  }
  //check types
  for(unsigned int i = 0; i < vtt.size(); ++i){

    if((*vtt[i] != reqd[i]) && ((reqd[i] == BOOL || *vtt[i] == BOOL)
                    || reqd[i] == INT)){
      cerr << "Invalid parameters for function " << *fname;
      cerr << " at line number " << lineNumber << "\n";
      exit(EXIT_FAILURE);
    }
  }

  return (TokenType*)lookup(*fname);
}
void *SAVisitor::visit(ASTNodeSubExpression *n){
  return (TokenType*)n->expression->accept(this);
}
void *SAVisitor::visit(ASTNodeUnary *n){
  TokenType *tt = (TokenType*)n->expression->accept(this);
  lineNumber = n->token->type;

  switch(*tt){
    case INT:
    case FLOAT:
      // unary operator must be 'MINUS' if float or int, not 'NOT'
      if(! (n->token->type == MINUS)) {
        cerr << "Invalid unary operator " << n->token->lexeme;
        cerr << " at line number " << n->token->lineNumber;
        cerr << ". Expression must be of boolean type\n";
        exit(EXIT_FAILURE);
      }
    break;
    case BOOL:
      // unary operator must be 'MINUS' if float or int, not 'NOT'
      if(! (n->token->type == NOT)){
        cerr << "Invalid unary operator " << n->token->lexeme;
        cerr << " at line number " << n->token->lineNumber;
        cerr << ". Expression must be of numeric type\n";
        exit(EXIT_FAILURE);
      }
    break;
    default:
    break;
  }
  return (TokenType*)tt;
}
void *SAVisitor::visit(ASTNodeFactor *n){
  if(ASTNodeIdentifier* n2 = dynamic_cast<ASTNodeIdentifier*>(n->node)){
    string name = *(string*)n2->accept(this);

    TokenType *tt = (TokenType*)lookup(name);
    if(tt == nullptr){
      cerr << "Identifier " << name << " at line ";
      cerr << lineNumber << " does not exist\n";
      exit(EXIT_FAILURE);
    }
    return tt;
  }
  return (TokenType*)n->node->accept(this);
}
void *SAVisitor::visit(ASTNodeTerm *n){
  // first factor is taken as reference
  // set as new since it value changes, but the reference shouldnt
  TokenType *refType = new TokenType(*(TokenType*)n->factors[0]->accept(this));

  for(unsigned int i = 0; i < n->multiplicativeOP.size(); ++i){
    TokenType multT = *(TokenType*)n->multiplicativeOP[i]->accept(this);
    // new type
    TokenType *newT = (TokenType*)n->factors[0]->accept(this);
    switch(multT){
      case TIMES:
      case DIVISION:
        if(*refType == BOOL || *newT == BOOL){
          cerr << "Type mismatch for operator " << Token::TokenString[multT];
          cerr << " at line " << lineNumber << "\n";
          exit(EXIT_FAILURE);
        }

        if(*refType == FLOAT || *newT == FLOAT) {
          *refType = FLOAT;
        }
        // else leave reftype as int, if both are integers
      break;
      case AND:
        if(*refType != BOOL || *newT != BOOL){ // both must be BOOL
          cerr << "Type mismatch for operator " << Token::TokenString[multT];
          cerr << " at line " << lineNumber << "\n";
          exit(EXIT_FAILURE);
        }
        // else leve reftype as bool
      break;
      default:
      break;
    }
  }

  return (TokenType*) refType;
}
void *SAVisitor::visit(ASTNodeSimpleExpression *n){
  // first factor is taken as reference
  TokenType *refType = new TokenType(*(TokenType*)n->terms[0]->accept(this));

  for(unsigned int i = 0; i < n->additiveOP.size(); ++i){
    TokenType addT = *(TokenType*)n->additiveOP[i]->accept(this);
    // new type
    TokenType *newT = (TokenType*)n->terms[i+1]->accept(this);
    switch(addT){
      case PLUS:
      case MINUS:
        if(*refType == BOOL || *newT == BOOL){
          cerr << "Type mismatch for operator " << Token::TokenString[addT];
          cerr << " at line " << lineNumber << "\n";
          exit(EXIT_FAILURE);
        }

        if(*refType == FLOAT || *newT == FLOAT) {
          *refType = FLOAT;
        }
        // else leave reftype as int, if both are integers
      break;
      case OR:
        if(*refType != BOOL || *newT != BOOL){ // both must be BOOL
          cerr << "Type mismatch for operator" << Token::TokenString[addT];
          cerr << " at line " << lineNumber << "\n";
          exit(EXIT_FAILURE);
        }
        // else leve reftype as bool
      break;
      default:
      break;
    }
  }

  return (TokenType*) refType;
}
void *SAVisitor::visit(ASTNodeExpression *n){
  // first factor is taken as reference
  TokenType *refType = new TokenType(*(TokenType*)n->simpleExpressions[0]->accept(this));

  // if there is a relop, then expression must be boolean
  for(unsigned int i = 0; i < n->relationalOp.size(); ++i){
    n->relationalOp[i]->accept(this); // just to set new line number
    // new type
    TokenType *newT = (TokenType*)n->simpleExpressions[i+1]->accept(this);
    if(*refType == BOOL || *newT == BOOL){
      cerr << "Type mismatch at line " << lineNumber << "\n";
      exit(EXIT_FAILURE);
    }
    *refType = BOOL;
  }

  return (TokenType*) refType;
}
void *SAVisitor::visit(ASTNodeAssignment *n){
  string name = *(string*)n->identifier->accept(this);

  if(functions.count(name)){ // if trying to assign a function
    cerr << name << " at line " << lineNumber;
    cerr << " already defined as  function";
    exit(EXIT_FAILURE);
  }
  
  TokenType type = *lookup(name);
  TokenType tt = *(TokenType*)n->expression->accept(this);

  if((tt != type) && ((type == BOOL || tt == BOOL)
                    || type == INT)){
    cerr << "Type mismatch at line " << lineNumber << "\n";
    exit(EXIT_FAILURE);
  }

 return 0;
}
void *SAVisitor::visit(ASTNodeVariableDecl *n){
  TokenType type = *(TokenType*)n->type->accept(this);
  string name = *(string*)n->identifier->accept(this);
  insert( name , type );

  TokenType tt = *(TokenType*)n->expression->accept(this);

  if((tt != type) && ((type == BOOL || tt == BOOL)
                    || type == INT)){
    cerr << "Type mismatch at line " << lineNumber << "\n";
    exit(EXIT_FAILURE);
  }
  return 0;
}
void *SAVisitor::visit(ASTNodePrintStatement *n){
  n->expression->accept(this);
  return 0;
}
void *SAVisitor::visit(ASTNodeReturnStatement *n){
  TokenType *tt = (TokenType*)n->expression->accept(this);

  if(currentFunctionType != nullptr && *currentFunctionType != *tt){
    cerr << "Bad return type at line " << lineNumber << "\n";
    exit(EXIT_FAILURE);
  }

  if(insideFunction && !insideFor){
    if(ifsReturnIndex == -1){
      goodReturn = true;
    }else{
      ifsReturn[ifsReturnIndex] = true;
    }
  }

  return 0;
}
void *SAVisitor::visit(ASTNodeIfStatement *n){
  TokenType tt = *(TokenType*)n->expression->accept(this);
  // expression must be true or false
  if(tt != BOOL){
    cerr << "Expression must be a condition at line " << lineNumber << "\n";
    exit(EXIT_FAILURE);
  }
  if(!insideFor && insideFunction){
    ifsReturn.insert(ifsReturn.begin(), false);
    ifsReturnIndex++;
  }

  newScope();
  n->block->accept(this);
  removeScope();

  if(n->elseBlock){
    if(!insideFor && insideFunction){
      ifsReturn.insert(ifsReturn.begin(), false);
    }

    newScope();
    n->elseBlock->accept(this);
    removeScope();

    if(!insideFor && insideFunction){
      if(ifsReturn[ifsReturnIndex] && ifsReturn[ifsReturnIndex+1]){
        if(ifsReturn.size() > 2){
          ifsReturn[ifsReturnIndex-1] = true;
        }else{
          goodReturn = true;
        }
      }

      ifsReturn.erase(ifsReturn.begin() + ifsReturnIndex);
      ifsReturn.erase(ifsReturn.begin() + ifsReturnIndex);
    }
  }else{
    if(!insideFor && insideFunction){
      ifsReturn.erase(ifsReturn.begin() + ifsReturnIndex);
    }
  }

  if(!insideFor && insideFunction){
    ifsReturnIndex--;
  } 

  return 0;
}
void *SAVisitor::visit(ASTNodeForStatement *n){
  newScope();
  insideFor = true;
  if(n->variableDecl) n->variableDecl->accept(this);

  TokenType tt = *(TokenType*)n->expression->accept(this);
  if(tt != BOOL){
    cerr << "Expression must be a condition at line " << lineNumber << "\n";
    exit(EXIT_FAILURE);
  }

  if(n->assignment) n->assignment->accept(this);

  n->block->accept(this);
  insideFor = false;
  removeScope();

  return 0;
}
void *SAVisitor::visit(ASTNodeFormalParam *n){
  string *name = (string*)n->identifier->accept(this);
  TokenType *ttl = (TokenType*)n->type->accept(this);
  insert( *name, *ttl );

  return (TokenType*)ttl; 
}
void *SAVisitor::visit(ASTNodeFormalParams *n){
  vector<TokenType> *tt = new vector<TokenType>();
  for(unsigned int i = 0; i < n->formalParams.size(); ++i){
    tt->push_back( *(TokenType*)n->formalParams[i]->accept(this) );
  }
  return (vector<TokenType>*)tt;
}
void *SAVisitor::visit(ASTNodeFunctionDecl *n){
  string *name = (string*)n->identifier->accept(this);
  TokenType *tt = (TokenType*)n->type->accept(this);
  insert(*name, *tt);

  if(scope.size() > 1){
    cerr << "Functions cannot be declared outside of main scope";
    cerr << " at line " + lineNumber << "\n";
    exit(EXIT_FAILURE);
  }

  currentFunctionType = new TokenType(*tt);
  functions[*name] = vector<TokenType>();
  insideFunction = true;
  newScope();

  if(n->formalParams){
    vector<TokenType> *vtt = (vector<TokenType>*)n->formalParams->accept(this);
	functions[*name] = *vtt;
  }
  n->block->accept(this);

  removeScope();
  if(!goodReturn){
	n->identifier->accept(this); // to get line number again
    cerr << "Function " << *name << " at line number " << lineNumber;
    cerr << ": not all code paths return a value\n";
    exit(EXIT_FAILURE);
  }
  goodReturn = false;
  insideFunction = false;

  currentFunctionType = nullptr;
  return 0;
}
void *SAVisitor::visit(ASTNodeStatement *n){
  // if it is a block, start new scope
  if(dynamic_cast<ASTNodeBlock*>(n->statement)){
    newScope();
    n->statement->accept(this);
    removeScope();
  }else{
    n->statement->accept(this);
  }

  return 0;
}
void *SAVisitor::visit(ASTNodeBlock *n){
  for(unsigned int i = 0; i < n->statements.size(); ++i){
    n->statements[i]->accept(this);
  }

  return 0;
}
void *SAVisitor::visit(ASTNodeProgram *n){
  newScope();
  // visit function declerations first
  for(unsigned int i = 0; i < n->statements.size(); ++i){
    ASTNodeStatement* n2 = dynamic_cast<ASTNodeStatement*>(n->statements[i]);
    if(dynamic_cast<ASTNodeFunctionDecl*>(n2->statement)){
      n->statements[i]->accept(this);
    }
  }
  for(unsigned int i = 0; i < n->statements.size(); ++i){
    ASTNodeStatement* n2 = dynamic_cast<ASTNodeStatement*>(n->statements[i]);
    if(! dynamic_cast<ASTNodeFunctionDecl*>(n2->statement)){
      n->statements[i]->accept(this);
    }
  }
  removeScope();

  return 0;
}
