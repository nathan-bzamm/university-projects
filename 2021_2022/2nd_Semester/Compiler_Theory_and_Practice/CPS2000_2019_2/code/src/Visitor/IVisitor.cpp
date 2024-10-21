#include <iostream>

#include "Visitor.h"

using std::map;
using std::cerr;
using std::cout;

void IVisitor::newScope(){
  scope.insert(scope.begin() ,map<string, ValueType>());
}
void IVisitor::insert(string s, ValueType vt){
  // if lexeme already exists in current scope or is a function name
  if(scope[0].count(s) || functions.count(s)){
    cerr << "Symbol " << s << " at line " << lineNumber;
    cerr << " already exists\n";
    exit(EXIT_FAILURE);
  }
  scope[0][s] = vt;
}
void IVisitor::removeScope(){
  scope.erase(scope.begin());
}
ValueType *IVisitor::lookup(string s){
  for(unsigned int i = 0; i < scope.size(); ++i){
    if(scope[i].count(s)){ // if key exists
      ValueType *vt = &scope[i].find(s)->second;
      return vt;
    }
  }
  return nullptr;
}
void IVisitor::printSymbolTable(){
  cout << "Symbol Table:\n";
  for(unsigned int i = 0; i < scope.size(); ++i){
    cout << "scope " << i << std::endl;
    map<string, ValueType>::iterator it;
    for ( it = scope[i].begin(); it != scope[i].end(); it++ ){
      cout << it->first << " : " << *(float*)it->second.value << ", " << Token::TokenString[it->second.type];
    }
    cout << "\n\n";
  }
  cout << "Functions Table:\n";
  map<string, ASTNode*>::iterator it;
  for ( it = functions.begin(); it != functions.end(); it++ ){
    cout << it->first << "\n";
  }
  cout << "\n";
}
void IVisitor::printValue(ValueType* vt){
  if(vt != NULL){
    switch(vt->type){
      case BOOL:
        if(*(float*)vt->value == 1){
          cout << "true";
        }else{
          cout << "false";
        }
      break;
      case INT:
      case FLOAT:
        cout << *(float*)vt->value;
      break;
      default:
      break;
    }
  }else{
    cout << "NULL";
  }
  cout << "\n";
}

void *IVisitor::visit(ASTNodeType *n){
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
void *IVisitor::visit(ASTNodeLiteral *n){
  lineNumber = n->token->lineNumber;
  ValueType *vt = new ValueType(0, n->token->type);
  switch(n->token->type){
    case BOOL:
    case INT:
    case FLOAT:
      vt->value = (float*)&n->token->number;
    break;
    default:
    break;
  }
  return (ValueType*)vt;
}
void *IVisitor::visit(ASTNodeIdentifier *n){
  lineNumber = n->token->lineNumber;
  return (string*)(&n->token->lexeme);
}
void *IVisitor::visit(ASTNodeMultiplicativeOp *n){
  lineNumber = n->token->lineNumber;
  return (TokenType*)&n->token->type;
}
void *IVisitor::visit(ASTNodeAdditiveOp *n){
  lineNumber = n->token->lineNumber;
  return (TokenType*)&n->token->type;
}
void *IVisitor::visit(ASTNodeRelationalOp *n){
  lineNumber = n->token->lineNumber;
  return (TokenType*)&n->token->type;
}
void *IVisitor::visit(ASTNodeActualParams *n){
  vector<ValueType> *values = new vector<ValueType>();
  for(unsigned int i = 0; i < n->expressions.size(); ++i){
    values->push_back(*(ValueType*)n->expressions[i]->accept(this));
  }

  return (vector<ValueType>*)values;
}
void *IVisitor::visit(ASTNodeFunctionCall *n){
  performFunction = true;

  if(n->actualParams){
    parameters = vector<ValueType>(*(vector<ValueType>*)n->actualParams->accept(this));
  }

  functions[*(string*)n->identifier->accept(this)]->accept(this);

  performFunction = false;
  parameters = vector<ValueType>();
  returnFromFunction = false;

  ValueType *vt = new ValueType(*returnValue);
  returnValue = NULL;

  return vt;
}
void *IVisitor::visit(ASTNodeSubExpression *n){
  return n->expression->accept(this);
}
void *IVisitor::visit(ASTNodeUnary *n){
  ValueType *expr = (ValueType*)n->expression->accept(this);

  TokenType unary = n->token->type;
  switch(unary){
    case MINUS:
      expr->value = new float( -(float)*(float*)expr->value );
    break;
    case NOT:
      expr->value = new float( !(float)*(float*)expr->value );
    break;
    default:
    break;
  }

  return (ValueType*)expr;
}
void *IVisitor::visit(ASTNodeFactor *n){
  if(ASTNodeIdentifier* n2 = dynamic_cast<ASTNodeIdentifier*>(n->node)){
    string name = *(string*)n2->accept(this);
    return (ValueType*)lookup(name);
  }
  return (ValueType*)n->node->accept(this);
}
void *IVisitor::visit(ASTNodeTerm *n){
  // first factor is taken as reference
  ValueType *refValue = (ValueType*)n->factors[0]->accept(this);

  for(unsigned int i = 0; i < n->multiplicativeOP.size(); ++i){
    TokenType multT = *(TokenType*)n->multiplicativeOP[i]->accept(this);

    // new type
    ValueType *newValue = (ValueType*)n->factors[i+1]->accept(this);
    switch(multT){
      void *toReturn;
      case TIMES:
        toReturn = new float(*(float*)refValue->value * *(float*)newValue->value);
        refValue = (ValueType*)(new ValueType(toReturn, FLOAT));
      break;
      case DIVISION:
        if(*(float*)newValue->value == 0){
          cerr << "Runtime error division by 0 at line " << lineNumber << "\n";
          exit(EXIT_FAILURE);
        }
        toReturn = new float(*(float*)refValue->value / *(float*)newValue->value);
        refValue = (ValueType*)(new ValueType(toReturn, FLOAT));
      break;
      case AND:
        toReturn = new float(*(float*)refValue->value && *(float*)newValue->value);
        refValue = (ValueType*)(new ValueType(toReturn, BOOL));
      break;
      default:
      break;
    }
  }
  return refValue;
}
void *IVisitor::visit(ASTNodeSimpleExpression *n){
  // first factor is taken as reference
  ValueType *refValue = (ValueType*)n->terms[0]->accept(this);

  for(unsigned int i = 0; i < n->additiveOP.size(); ++i){
    TokenType multT = *(TokenType*)n->additiveOP[i]->accept(this);

    // new type
    ValueType *newValue = (ValueType*)n->terms[i+1]->accept(this);
    switch(multT){
      void *toReturn;
      case PLUS:
        toReturn = new float(*(float*)refValue->value + *(float*)newValue->value);
        refValue = (ValueType*)(new ValueType(toReturn, FLOAT));
      break;
      case MINUS:
        toReturn = new float(*(float*)refValue->value - *(float*)newValue->value);
        refValue = (ValueType*)(new ValueType(toReturn, FLOAT));
      break;
      case OR:
        toReturn = new float(*(float*)refValue->value || *(float*)newValue->value);
        refValue = (ValueType*)(new ValueType(toReturn, BOOL));
      break;
      default:
      break;
    }
  }
  return refValue;
}
void *IVisitor::visit(ASTNodeExpression *n){
  // first factor is taken as reference
  ValueType *refValue = (ValueType*)n->simpleExpressions[0]->accept(this);

  // if there is a relop, then expression must be boolean
  for(unsigned int i = 0; i < n->relationalOp.size(); ++i){
    TokenType tt = *(TokenType*)n->relationalOp[i]->accept(this); // just to set new line number
    // new type
    ValueType *newValue = (ValueType*)n->simpleExpressions[i+1]->accept(this);

    float *b;

    switch (tt)
    {
      case ST:
        b = new float( *(float*)refValue->value < *(float*)newValue->value );
      break;
      case GT:
        b = new float( *(float*)refValue->value > *(float*)newValue->value );
      break;
      case EQQ:
        b = new float( *(float*)refValue->value == *(float*)newValue->value );
      break;
      case NE:
        b = new float( *(float*)refValue->value != *(float*)newValue->value );
      break;
      case SE:
        b = new float( *(float*)refValue->value <= *(float*)newValue->value );
      break;
      case GE:
        b = new float( *(float*)refValue->value >= *(float*)newValue->value );
      break;
      default:
      break;
    }

    refValue = new ValueType(b ,BOOL);
  }

  return (ValueType*) refValue;
}
void *IVisitor::visit(ASTNodeAssignment *n){
  string name = *(string*)n->identifier->accept(this);

  ValueType *vt = lookup(name);

  vt->value = ((ValueType*)n->expression->accept(this))->value;
  
  if(vt->type == INT){
    // if it is an int, first convert it to a float, then to an int then to a float again
    int i = (int)(*(float*)vt->value);
    vt->value = new float(i);
  }

  return 0;
}
void *IVisitor::visit(ASTNodeVariableDecl *n){
  TokenType type = *(TokenType*)n->type->accept(this);
  string name = *(string*)n->identifier->accept(this);

  ValueType *vt = new ValueType(((ValueType*)n->expression->accept(this))->value, type);
  
  if(vt->type == INT){
    // if it is an int, first convert it to a float, then to an int then to a float again
    int i = (int)(*(float*)vt->value);
    vt->value = new float(i);
  }

  insert( name , *vt );

  return 0;
}
void *IVisitor::visit(ASTNodePrintStatement* n){
  printValue((ValueType*)n->expression->accept(this));
  return 0;
}
void *IVisitor::visit(ASTNodeReturnStatement *n){
  returnValue = new ValueType(*(ValueType*)n->expression->accept(this));
  returnFromFunction = true;

  return 0;
}
void *IVisitor::visit(ASTNodeIfStatement *n){
  float boolean = *(float*)((ValueType*)n->expression->accept(this))->value;

  if(boolean){
    newScope();
    n->block->accept(this);
    removeScope();
  }else{
    if(n->elseBlock){
      newScope();
      n->elseBlock->accept(this);
      removeScope();
    }
  }

  return 0;
}
void *IVisitor::visit(ASTNodeForStatement *n){
  newScope();
  if(n->variableDecl) n->variableDecl->accept(this);

  float boolean = *(float*)((ValueType*)n->expression->accept(this))->value;
  while(boolean == true && returnFromFunction == false){
    newScope();
    n->block->accept(this);
    if(n->assignment) n->assignment->accept(this);
    boolean = *(float*)((ValueType*)n->expression->accept(this))->value;
    removeScope();
  }
  
  removeScope();

  return 0;
}
void *IVisitor::visit(ASTNodeFormalParam *n){
  return (string*)n->identifier->accept(this);
}
void *IVisitor::visit(ASTNodeFormalParams *n){
  vector<string> *names = new vector<string>();
  for(unsigned int i = 0; i < n->formalParams.size(); ++i){
    names->push_back( *(string*)n->formalParams[i]->accept(this) );
  }
  return (vector<string>*)names;
}
void *IVisitor::visit(ASTNodeFunctionDecl *n){
  if(performFunction == false){
    string name = *(string*)n->identifier->accept(this);
    functions[name] = n;
  }else{
    newScope();
    if(n->formalParams){
      vector<string> params = *(vector<string>*)n->formalParams->accept(this);
      for(unsigned int i = 0; i < params.size(); ++i){
        insert( params[i], parameters[i] );
      }
    }
    n->block->accept(this);
    removeScope();
  }
  return 0;
}
void *IVisitor::visit(ASTNodeStatement *n){
  if(returnFromFunction == false){
    return n->statement->accept(this);
  }
  return 0;
}
void *IVisitor::visit(ASTNodeBlock *n){
  if(returnFromFunction == false){
    for(unsigned int i = 0; i < n->statements.size(); ++i){
      n->statements[i]->accept(this);
    }
  }
  return 0;
}
void *IVisitor::visit(ASTNodeProgram *n){
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
