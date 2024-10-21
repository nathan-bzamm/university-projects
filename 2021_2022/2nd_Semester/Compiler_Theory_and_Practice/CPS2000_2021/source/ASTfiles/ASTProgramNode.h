//
// Created by aidan on 07/05/2019.
//

#pragma once

#include "AST.h"

class ASTProgramNode:public ASTNode{
private:
    std::vector<ASTStatementNode*>* program;
public:
    ASTProgramNode(std::vector<ASTStatementNode*>* startingStatement);
    ~ASTProgramNode();

    virtual void Accept(Visitor* v);

    std::vector<ASTStatementNode *> *getProgram() const;
};
