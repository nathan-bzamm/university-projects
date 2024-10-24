#include "Visitor.h"

#include <iostream>

string XMLVisitor::tabsString(){
    if(xml.str().back() == ' ') return " ";
    string ret;
    for(unsigned int i = 0; i < numOfTabs; i++){
        //one tab is 2 spaces
        ret.append("  ");
    }
    return ret;
}

void *XMLVisitor::trimXMLNewLines(){
    string s = xml.str();

    unsigned int newLineCount = 0, numberToDelete = 1;
    for(unsigned int i = 0; i < s.length(); i++){
        if(s.at(i) == '\n'){
            newLineCount++;
            if(newLineCount == 2){
                s.erase(i-numberToDelete, numberToDelete);
                i--;
                newLineCount = 1;
            }
        }else if(s.at(i) != ' '){
            newLineCount = 0;
            numberToDelete = 1;
        }else if(s.at(i) == ' '){
            numberToDelete++;
        }
    }

    xml.str("");
    xml << s;
    return 0;
}

void *XMLVisitor::visit(ASTNodeType *n){
    xml << "Type = \"";
    switch(n->token->type){
      case TYPE_FLOAT:
        xml << "float";
      break;
      case TYPE_INT:
        xml << "int";
      break;
      case TYPE_BOOL:
        xml << "bool";
      break;
      case TYPE_CHAR:
        xml << "char";
      default:
      break;
    }
    xml << "\"";
    return 0;
}

void *XMLVisitor::visit(ASTNodeLiteral *n){
  switch(n->token->type){
    case FLOAT:
      xml << "<FloatConst>" << n->token->number << "</FloatConst>";
    break;
    case INT:
      xml << "<IntConst>" << n->token->number << "</IntConst>";
    break;
    case BOOL:
      xml << "<BoolConst>" << n->token->lexeme << "</BoolConst>";
    break;
    case CHAR:
      xml << "<CharConst>" << n->token->lexeme << "</CharConst>";
    default:
    break;
  }
  return 0;
}
void *XMLVisitor::visit(ASTNodeIdentifier *n){
  xml << n->token->lexeme << "</ID>";
  return 0;
}
void *XMLVisitor::visit(ASTNodeMultiplicativeOp *n){
  xml << "OP=\"" << n->token->lexeme << "\"";
  return 0;
}
void *XMLVisitor::visit(ASTNodeAdditiveOp *n){
  xml << "OP=\"" << n->token->lexeme << "\"";
  return 0;
}
void *XMLVisitor::visit(ASTNodeRelationalOp *n){
  xml << "OP=\"" << n->token->lexeme << "\"";
  return 0;
}
void *XMLVisitor::visit(ASTNodeActualParams *n){
  for(unsigned int i = 0; i < n->expressions.size(); ++i){
    xml << "\n" << tabsString() << "<A_Param>\n";
    numOfTabs++;
    n->expressions.at(i)->accept(this);
    numOfTabs--;
    xml << "\n" << tabsString() << "</A_Param>\n";
  }
  return 0;
}
void *XMLVisitor::visit(ASTNodeFunctionCall *n){
  xml << tabsString() <<"<FN_CALL FN=";
  n->identifier->accept(this);
  xml << "\"\n";
  numOfTabs++;
  if(n->actualParams){
    n->actualParams->accept(this);
  }
  numOfTabs--;
  xml << "\n" << tabsString() << "</FN_CALL FN>";
  return 0;
}
void *XMLVisitor::visit(ASTNodeSubExpression *n){
  n->expression->accept(this);
  return 0;
}
void *XMLVisitor::visit(ASTNodeUnary *n){
  xml << "\n" << tabsString() << "UnaryExpr OP=\"";
  xml << ((n->token->type == MINUS)? "-" : "not");
  xml << "\"";
  
  xml << ">\n";
  numOfTabs++;
  xml << tabsString();
  n->expression->accept(this);
  xml << "\n";
  numOfTabs--;
  xml << tabsString() << "</UnaryExpr>\n";
  return 0;
}
void *XMLVisitor::visit(ASTNodeFactor *n){
  n->node->accept(this);
  return 0;
}
void *XMLVisitor::visit(ASTNodeTerm *n){
  unsigned int factorsIndex = 0;
  for(unsigned int multOpIndex = 0; multOpIndex < n->multiplicativeOP.size(); ++multOpIndex){
    xml << "\n" << tabsString() << "<BinExpr ";
    n->multiplicativeOP.at(multOpIndex)->accept(this);
    xml << ">\n";
    numOfTabs++;
    xml << tabsString();
    n->factors.at(factorsIndex++)->accept(this);
    xml << "\n";
  }
  xml << tabsString();
  n->factors.at(factorsIndex++)->accept(this);
  xml << "\n";
  for(unsigned int multOpIndex = 0; multOpIndex < n->multiplicativeOP.size(); ++multOpIndex){
    numOfTabs--;
    xml << tabsString() << "</BinExpr>\n";
  }
  return 0;
}
void *XMLVisitor::visit(ASTNodeSimpleExpression *n){
  unsigned int termsIndex = 0;
  for(unsigned int multOpIndex = 0; multOpIndex < n->additiveOP.size(); ++multOpIndex){
    xml << tabsString() << "BinExprNode ";
    n->additiveOP.at(multOpIndex)->accept(this);
    xml << ">\n";
    numOfTabs++;
    xml << tabsString();
    n->terms.at(termsIndex++)->accept(this);
    xml << "\n";
  }
  xml << tabsString();
  n->terms.at(termsIndex++)->accept(this);
  xml << "\n";
  for(unsigned int multOpIndex = 0; multOpIndex < n->additiveOP.size(); ++multOpIndex){
    numOfTabs--;
    xml << tabsString() << "</BinExprNode>\n";
  }
  return 0;
}
void *XMLVisitor::visit(ASTNodeExpression *n){
  unsigned int factorsIndex = 0;
  for(unsigned int multOpIndex = 0; multOpIndex < n->relationalOp.size(); ++multOpIndex){
    xml << tabsString() << "<BinExprNode ";
    n->relationalOp.at(multOpIndex)->accept(this);
    xml << ">\n";
    numOfTabs++;
    xml << tabsString();
    n->simpleExpressions.at(factorsIndex++)->accept(this);
    xml << "\n";
  }
  xml << tabsString();
  n->simpleExpressions.at(factorsIndex++)->accept(this);
  xml << "\n";
  for(unsigned int multOpIndex = 0; multOpIndex < n->relationalOp.size(); ++multOpIndex){
    numOfTabs--;
    xml << tabsString() << "</BinExprNode>\n";
  }
  return 0;
}
void *XMLVisitor::visit(ASTNodeAssignment *n){
  xml << "\n" << tabsString() << "<Assign>\n";
  numOfTabs++;
  xml << tabsString();
  n->identifier->accept(this);
  xml << "\n";
  xml << tabsString();
  n->expression->accept(this);
  numOfTabs--;
  xml << "\n" << tabsString() << "</Assign>\n";
  return 0;
}
void *XMLVisitor::visit(ASTNodeVariableDecl *n){
  xml << "\n" << tabsString() << "<VarDecl>\n";
  numOfTabs++;
  xml << tabsString() << "<Var ";
  n->type->accept(this);
  xml << ">";
  n->identifier->accept(this);
  xml << "\n";
  n->expression->accept(this);
  numOfTabs--;
  xml << "\n" << tabsString() << "</VarDecl>\n";
  return 0;
}
void *XMLVisitor::visit(ASTNodePrintStatement *n){
  xml << "\n" << tabsString() << "<Print>\n";
  numOfTabs++;
  n->expression->accept(this);
  numOfTabs--;
  xml << "\n" << tabsString() << "</Print>\n";
  return 0;
}
void *XMLVisitor::visit(ASTNodeReturnStatement *n){
  xml << "\n" << tabsString() << "<Return>\n";
  numOfTabs++;
  n->expression->accept(this);
  numOfTabs--;
  xml << "\n" << tabsString() << "</Return>\n";
  return 0;
}
void *XMLVisitor::visit(ASTNodeIfStatement *n){
  xml << "\n" << tabsString() << "<IF>\n";
  numOfTabs++;
  n->expression->accept(this);
  numOfTabs--;
  xml << "\n" << tabsString() << "<THEN>\n";
  numOfTabs++;
  n->block->accept(this);
  numOfTabs--;
  if(n->elseBlock){
    xml << "\n" << tabsString() << "<ELSE>\n";
    numOfTabs++;
    n->elseBlock->accept(this);
    numOfTabs--;
  }
  xml << "\n" << tabsString() << "<END IF>\n";
  return 0;
}
void *XMLVisitor::visit(ASTNodeForStatement *n){
  xml << "\n" << tabsString() << "<FOR>\n";
  numOfTabs++;
  if(n->variableDecl) n->variableDecl->accept(this);
  n->expression->accept(this);
  if(n->assignment) n->assignment->accept(this);
  numOfTabs--;
  xml << "\n" << tabsString() << "<DO>\n";
  numOfTabs++;
  n->block->accept(this);
  numOfTabs--;
  xml << "\n" << tabsString() << "<ENDFOR>\n";
  return 0;
}
void *XMLVisitor::visit(ASTNodeFormalParam *n){
  xml << "\n" << tabsString() << "<F_Param> ";
  numOfTabs++;
  n->identifier->accept(this);
  xml << ":";
  n->type->accept(this);
  numOfTabs--;
  xml << " </F_Param>\n";
  return 0;
}
void *XMLVisitor::visit(ASTNodeFormalParams *n){
  for(unsigned int i = 0; i < n->formalParams.size(); ++i){
    n->formalParams.at(i)->accept(this);
  }
  return 0;
}
void *XMLVisitor::visit(ASTNodeFunctionDecl *n){
  xml << "\n" << tabsString() << "<FuncDecl>\n";
  numOfTabs++;
  xml << tabsString() << "<FN ";
  n->type->accept(this);
  xml << ">";
  n->identifier->accept(this);
  xml << "\n";
  if(n->formalParams){
    n->formalParams->accept(this);
  }
  xml << "\n";
  n->block->accept(this);
  numOfTabs--;
  xml << "\n" << tabsString() << "</FuncDecl>\n";
  return 0;
}
void *XMLVisitor::visit(ASTNodeStatement *n){
  n->statement->accept(this);
  return 0;
}
void *XMLVisitor::visit(ASTNodeBlock *n){
  for(unsigned int i = 0; i < n->statements.size(); ++i){
    n->statements.at(i)->accept(this);
  }
  return 0;
}
void *XMLVisitor::visit(ASTNodeProgram *n){
  for(unsigned int i = 0; i < n->statements.size(); ++i){
    n->statements.at(i)->accept(this);
  }
  return 0;
}
