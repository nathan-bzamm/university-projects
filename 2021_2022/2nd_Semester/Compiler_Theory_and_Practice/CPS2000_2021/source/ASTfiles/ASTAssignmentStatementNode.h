//
// Created by aidan on 06/05/2019.
//
#pragma once

#include <string>
#include "AST.h"

class ASTAssignmentStatementNode:public ASTStatementNode{
private:
    std::string idName;
    ASTExpressionNode* result;

public:

    ASTAssignmentStatementNode(const std::string &idName, ASTExpressionNode* result);
    ~ASTAssignmentStatementNode();

    virtual void Accept(Visitor* v);

    const std::string &getIdName() const;

    void setIdName(const std::string &idName);

    ASTExpressionNode *getResult() const;

    void setResult(ASTExpressionNode *result);
};