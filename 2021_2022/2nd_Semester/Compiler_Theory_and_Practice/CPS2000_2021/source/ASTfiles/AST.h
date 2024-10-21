//
// Created by aidan on 30/04/2019.
//
#pragma once

#include <string>
#include <vector>
#include "../Visitor.h"
//#include "ASTVar

#ifndef CPS2000_COMPILERS_ASSIGNMENT_AST_H
#define CPS2000_COMPILERS_ASSIGNMENT_AST_H

class ASTNode{
public:
    ASTNode();
    virtual ~ASTNode();
};

class ASTStatementNode: public ASTNode{
public:
    ASTStatementNode();
    ~ASTStatementNode() override;
    virtual void Accept(Visitor* v);
};


class ASTExpressionNode:public ASTStatementNode{
public:
    ASTExpressionNode();
    ~ASTExpressionNode() override;
    virtual void Accept(Visitor* v);
};

//class ASTVarDeclNode: public ASTNode
//{
//public:
//    ASTVarDeclNode(const std::string &name,const  std::string &type, ASTExpressionNode* value);
//    ~ASTVarDeclNode();
//
//private:
//    std::string varName;
//    std::string type;
//    ASTExpressionNode* value;
//};



#endif //CPS2000_COMPILERS_ASSIGNMENT_AST_H
