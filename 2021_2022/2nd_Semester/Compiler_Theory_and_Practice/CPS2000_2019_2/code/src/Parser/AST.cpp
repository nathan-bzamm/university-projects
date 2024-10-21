#include <iostream>

#include "AST.h"

using std::cerr;

Token *ASTNode::match(TokenType tt){
  Token *token = tokenManager->nextToken();
  if(token == nullptr || token->type != tt){
    cerr << "Error, could not match " << Token::TokenString[tt];
    cerr << " at line " << tokenManager->peekToken(-1)->lineNumber << "\n";
	  exit(EXIT_FAILURE);
  }else{
    return token;
  }
}

// Type Node
bool ASTNodeType::parse(){
  switch(tokenManager->peekToken()->type){
    case TYPE_FLOAT:
    case TYPE_INT:
    case TYPE_BOOL:
      token = tokenManager->nextToken();
      return true;
    break;
    default:
      return false;
  }
}

// Literal Node
bool ASTNodeLiteral::parse(){
  switch(tokenManager->peekToken()->type){
    case FLOAT:
    case INT:
    case BOOL:
      token = tokenManager->nextToken();
      return true;
    break;
    default:
      return false;
  }
}

// Literal Node
bool ASTNodeIdentifier::parse(){
  token = match(ID);
  return true;
}

// MultiplictiveOp Node
bool ASTNodeMultiplicativeOp::parse(){
  switch(tokenManager->peekToken()->type){
    case TIMES:
    case DIVISION:
    case AND:
      token = tokenManager->nextToken();
      return true;
    break;
    default:
      return false;
  }
}

// AdditiveOp Node
bool ASTNodeAdditiveOp::parse(){
  switch(tokenManager->peekToken()->type){
    case PLUS:
    case MINUS:
    case OR:
      token = tokenManager->nextToken();
      return true;
    break;
    default:
      return false;
  }
}

// AdditiveOp Node
bool ASTNodeRelationalOp::parse(){
  switch(tokenManager->peekToken()->type){
    case ST:
    case GT:
    case EQQ:
    case NE:
    case SE:
    case GE:
      token = tokenManager->nextToken();
      return true;
    break;
    default:
      return false;
  }
}

// ActualParams Node
bool ASTNodeActualParams::parse(){
  ASTNode *expr = new ASTNodeExpression(tokenManager);
  if (expr->parse() == false) return false; // expression must be valid
  expressions.push_back(expr); // At least one

  while(tokenManager->peekToken()->type == COMMA){
    match(COMMA);
    ASTNode* expr = new ASTNodeExpression(tokenManager);
    if (expr->parse() == false) return false; // expression must be valid
    expressions.push_back(expr);
  }
  return true;
}

// FunctionCall Node
bool ASTNodeFunctionCall::parse(){
  ASTNode *id= new ASTNodeIdentifier(tokenManager);
  if (id->parse() == false) return false;
  identifier = id;
  
  match(OPEN_BRACKET);

  // peeking to check if there is an actualparam or not
  if(tokenManager->peekToken()->type != CLOSED_BRACKET){
    ASTNode *actualP= new ASTNodeActualParams(tokenManager);
    if (actualP->parse() == false) return false;
    actualParams = actualP;
  }

  match(CLOSED_BRACKET);
  return true;
}
ASTNodeFunctionCall::~ASTNodeFunctionCall(){
  delete actualParams;
}

// SubExpression Node
bool ASTNodeSubExpression::parse(){
  match(OPEN_BRACKET);
  ASTNode *expr= new ASTNodeExpression(tokenManager);
  if (expr->parse() == false) return false;
  expression = expr;
  match(CLOSED_BRACKET);
  return true;
}
ASTNodeSubExpression::~ASTNodeSubExpression(){
  delete expression;
}

// Unary Node
bool ASTNodeUnary::parse(){
  switch (tokenManager->peekToken()->type){
    case MINUS:
      token = match(MINUS);
    break;
    case NOT:
      token = match(NOT);
    break;
    default:
      return false;
  }

  ASTNode *expr= new ASTNodeExpression(tokenManager);
  if (expr->parse() == false) return false;
  expression = expr;

  return true;
}
ASTNodeUnary::~ASTNodeUnary(){
  delete expression;
}

// Factor Node
bool ASTNodeFactor::parse(){
  ASTNode *n;
  switch (tokenManager->peekToken()->type)
  {
    case FLOAT:
    case INT:
    case BOOL:
      n = new ASTNodeLiteral(tokenManager);  
    break;
    case ID:
      if(tokenManager->peekToken(1)->type == OPEN_BRACKET){
        n = new ASTNodeFunctionCall(tokenManager);
      }else{
        n = new ASTNodeIdentifier(tokenManager);
      }
    break;
    case OPEN_BRACKET:
      n = new ASTNodeSubExpression(tokenManager);
    break;
    case MINUS:
    case NOT:
      n = new ASTNodeUnary(tokenManager);
    break;
    default:
      return false;
  }
  if (n->parse() == false) return false;
  node = n;
  return true;
}
ASTNodeFactor::~ASTNodeFactor(){
  delete node;
}

// Term Node
bool ASTNodeTerm::parse(){
  ASTNode *n = new ASTNodeFactor(tokenManager);
  if (n->parse() == false) return false;
  factors.push_back(n); // at least one

  while(true){
    ASTNode *n2, *n3;
    switch (tokenManager->peekToken()->type){
      case TIMES:
      case DIVISION:
      case AND:
        n2 = new ASTNodeMultiplicativeOp(tokenManager);
        if (n2->parse() == false) return false;
        multiplicativeOP.push_back(n2);

        n3 = new ASTNodeFactor(tokenManager);
        if (n3->parse() == false) return false;
        factors.push_back(n3);
      break;
      default:
        return true;
    }
  }
}
ASTNodeTerm::~ASTNodeTerm(){
  for(uint8_t i = 0; i < factors.size(); ++i){
    delete factors[i];
  }
  for(uint8_t i = 0; i < multiplicativeOP.size(); ++i){
    delete multiplicativeOP[i];
  }
}

bool ASTNodeSimpleExpression::parse(){
  ASTNode *n = new ASTNodeTerm(tokenManager);
  if (n->parse() == false) return false;
  terms.push_back(n); // at least one

  while(true){
    ASTNode *n2, *n3;
    switch (tokenManager->peekToken()->type){
      case PLUS:
      case MINUS:
      case OR:
        n2 = new ASTNodeAdditiveOp(tokenManager);
        if (n2->parse() == false) return false;
        additiveOP.push_back(n2);

        n3 = new ASTNodeTerm(tokenManager);
        if (n3->parse() == false) return false;
        terms.push_back(n3);
      break;
      default:
        return true;
    }
  }
}
ASTNodeSimpleExpression::~ASTNodeSimpleExpression(){
  for(uint8_t i = 0; i < terms.size(); ++i){
    delete terms[i];
  }
  for(uint8_t i = 0; i < additiveOP.size(); ++i){
    delete additiveOP[i];
  }
}

// Expression Node
bool ASTNodeExpression::parse(){
  ASTNode *n = new ASTNodeSimpleExpression(tokenManager);
  if (n->parse() == false) return false;
  simpleExpressions.push_back(n); // at least one

  while(true){
    ASTNode *n2, *n3;
    switch (tokenManager->peekToken()->type){
      case ST:
      case GT:
      case EQQ:
      case NE:
      case SE:
      case GE:
        n2 = new ASTNodeRelationalOp(tokenManager);
        if (n2->parse() == false) return false;
        relationalOp.push_back(n2);

        n3 = new ASTNodeSimpleExpression(tokenManager);
        if (n3->parse() == false) return false;
        simpleExpressions.push_back(n3);
      break;
      default:
        return true;
    }
  }
}
ASTNodeExpression::~ASTNodeExpression(){
  for(uint8_t i = 0; i < simpleExpressions.size(); ++i){
    delete simpleExpressions[i];
  }
  for(uint8_t i = 0; i < relationalOp.size(); ++i){
    delete relationalOp[i];
  }
}

// Assignment Node
bool ASTNodeAssignment::parse(){
  ASTNode *n = new ASTNodeIdentifier(tokenManager);
  if (n->parse() == false) return false;
  identifier = n;

  match(EQ);

  ASTNode *n2 = new ASTNodeExpression(tokenManager);
  if (n2->parse() == false) return false;
  expression = n2;

  return true;
}
ASTNodeAssignment::~ASTNodeAssignment(){
  delete identifier;
  delete expression;
}

// VariableDecl Node
bool ASTNodeVariableDecl::parse(){
  match(VAR);

  ASTNode *n = new ASTNodeIdentifier(tokenManager);
  if (n->parse() == false) return false;
  identifier = n;

  match(COLON);

  ASTNode *n2 = new ASTNodeType(tokenManager);
  if (n2->parse() == false) return false;
  type = n2;

  match(EQ);

  ASTNode *n3 = new ASTNodeExpression(tokenManager);
  if (n3->parse() == false) return false;
  expression = n3;

  return true;
}
ASTNodeVariableDecl::~ASTNodeVariableDecl(){
  delete identifier;
  delete type;
  delete expression;
}

// PrintStatement Node
bool ASTNodePrintStatement::parse(){
  match(PRINT);

  ASTNode *n = new ASTNodeExpression(tokenManager);
  if (n->parse() == false) return false;
  expression = n;

  return true;
}
ASTNodePrintStatement::~ASTNodePrintStatement(){
  delete expression;
}

// ReturnStatement Node
bool ASTNodeReturnStatement::parse(){
  match(RETURN);

  ASTNode *n = new ASTNodeExpression(tokenManager);
  if (n->parse() == false) return false;
  expression = n;

  return true;
}
ASTNodeReturnStatement::~ASTNodeReturnStatement(){
  delete expression;
}

// IfStatement Node
bool ASTNodeIfStatement::parse(){
  match(IF);
  match(OPEN_BRACKET);

  ASTNode *n = new ASTNodeExpression(tokenManager);
  if (n->parse() == false) return false;
  expression = n;
  
  match(CLOSED_BRACKET);

  ASTNode *n2 = new ASTNodeBlock(tokenManager);
  if (n2->parse() == false) return false;
  block = n2;

  if(tokenManager->peekTokenUnsafe(0) != nullptr){
    if(tokenManager->peekToken()->type == ELSE){
      match(ELSE);

      ASTNode *n3 = new ASTNodeBlock(tokenManager);
      if (n3->parse() == false) return false;
      elseBlock = n3;
    }
  }

  return true;
}
ASTNodeIfStatement::~ASTNodeIfStatement(){
  delete expression;
  delete block;
  delete elseBlock;
}

// ForStatement Node
bool ASTNodeForStatement::parse(){
  match(FOR);
  match(OPEN_BRACKET);

  if(tokenManager->peekToken()->type == VAR){
    ASTNode *n = new ASTNodeVariableDecl(tokenManager);
    if (n->parse() == false) return false;
    variableDecl = n;
  }

  match(SEMI_COLON);

  ASTNode *n2 = new ASTNodeExpression(tokenManager);
  if (n2->parse() == false) return false;
  expression = n2;

  match(SEMI_COLON);

  if(tokenManager->peekToken()->type == ID){
    ASTNode *n3 = new ASTNodeAssignment(tokenManager);
    if (n3->parse() == false) return false;
    assignment = n3;
  }

  match(CLOSED_BRACKET);

  ASTNode *n4 = new ASTNodeBlock(tokenManager);
  if (n4->parse() == false) return false;
  block = n4;

  return true;
}
ASTNodeForStatement::~ASTNodeForStatement(){
  delete variableDecl;
  delete expression;
  delete assignment;
  delete block;
}

// FormalParam Node
bool ASTNodeFormalParam::parse(){
  ASTNode *n = new ASTNodeIdentifier(tokenManager);
  if (n->parse() == false) return false;
  identifier = n;

  match(COLON);

  ASTNode *n2 = new ASTNodeType(tokenManager);
  if (n2->parse() == false) return false;
  type = n2;

  return true;
}
ASTNodeFormalParam::~ASTNodeFormalParam(){
  delete identifier;
  delete type;
}

// FormalParams Node
bool ASTNodeFormalParams::parse(){
  ASTNode *n = new ASTNodeFormalParam(tokenManager);
  if (n->parse() == false) return false;
  formalParams.push_back(n); 

  while(tokenManager->peekToken()->type == COMMA){
    match(COMMA);

    ASTNode *n2 = new ASTNodeFormalParam(tokenManager);
    if (n2->parse() == false) return false;
    formalParams.push_back(n2);
  }

  return true;
}
ASTNodeFormalParams::~ASTNodeFormalParams(){
  for(uint8_t i = 0; i < formalParams.size(); ++i){
    delete formalParams[i];
  }
}

// FunctionDecl Node
bool ASTNodeFunctionDecl::parse(){
  match(FN);
  
  ASTNode *n = new ASTNodeIdentifier(tokenManager);
  if (n->parse() == false) return false;
  identifier = n;
  
  match(OPEN_BRACKET);
  
  if(tokenManager->peekToken()->type == ID){
    ASTNode *n2 = new ASTNodeFormalParams(tokenManager);
    if (n2->parse() == false) return false;
    formalParams = n2;
  }
  
  match(CLOSED_BRACKET);
  match(COLON);

  ASTNode *n3 = new ASTNodeType(tokenManager);
  if (n3->parse() == false) return false;
  type = n3;

  ASTNode *n4 = new ASTNodeBlock(tokenManager);
  if (n4->parse() == false) return false;
  block = n4;

  return true;
}
ASTNodeFunctionDecl::~ASTNodeFunctionDecl(){
  delete formalParams;
}

// Statement Node
bool ASTNodeStatement::parse(){
  ASTNode* n;
  TokenType tt = tokenManager->peekToken()->type;
  switch(tt){
    case VAR:
      n = new ASTNodeVariableDecl(tokenManager);
    break;
    case ID:
      n = new ASTNodeAssignment(tokenManager);
    break;
    case PRINT:
      n = new ASTNodePrintStatement(tokenManager);
    break;
    case IF:
      n = new ASTNodeIfStatement(tokenManager);
    break;
    case FOR:
      n = new ASTNodeForStatement(tokenManager);
    break;
    case RETURN:
      n = new ASTNodeReturnStatement(tokenManager);
    break;
    case FN:
      n = new ASTNodeFunctionDecl(tokenManager);
    break;
    case OPEN_BRACE:
      n = new ASTNodeBlock(tokenManager);
    break;
    default:
      return false;
  }

  if(n->parse() == false) return false;
  statement = n;

  switch(tt){
    case VAR:
    case ID:
    case RETURN:
    case PRINT:
      match(SEMI_COLON);
    break;
    default:
    break;
  }

  return true;
}
ASTNodeStatement::~ASTNodeStatement(void){
  delete statement;
}

// Block Node
bool ASTNodeBlock::parse(){
  match(OPEN_BRACE);

  while(tokenManager->peekToken() != nullptr && tokenManager->peekToken()->type != CLOSED_BRACE){ // parse multiple statements
    ASTNode *statement = new ASTNodeStatement(tokenManager);
    if (statement->parse() == false) return false; 
    statements.push_back(statement);
  }

  match(CLOSED_BRACE);
  
  return true;
}
ASTNodeBlock::~ASTNodeBlock(){
  for(uint8_t i = 0; i < statements.size(); ++i){
    delete statements[i];
  }
}

// Program Node
bool ASTNodeProgram::parse(){
  while(tokenManager->peekTokenUnsafe(0) != nullptr){
    ASTNode *statement = new ASTNodeStatement(tokenManager);
    if (statement->parse() == false) return false; 
    statements.push_back(statement);
  }
  return true;
}
ASTNodeProgram::~ASTNodeProgram(){
  for(uint8_t i = 0; i < statements.size(); ++i){
    delete statements[i];
  }
}

void *ASTNode::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeType::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeLiteral::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeIdentifier::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeMultiplicativeOp::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeAdditiveOp::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeRelationalOp::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeActualParams::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeFunctionCall::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeSubExpression::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeUnary::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeFactor::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeTerm::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeSimpleExpression::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeExpression::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeAssignment::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeVariableDecl::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodePrintStatement::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeReturnStatement::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeIfStatement::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeForStatement::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeFormalParam::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeFormalParams::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeFunctionDecl::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeStatement::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeBlock::accept(Visitor *v){
  return v->visit(this);
}
void *ASTNodeProgram::accept(Visitor *v){
  return v->visit(this);
}
