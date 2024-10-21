//
// Created by aidan on 10/05/2019.
//

#include "AST.h"

class ASTPrintStatementNode:public ASTStatementNode{
public:
    ASTExpressionNode *getPrintStatement() const;
    ASTPrintStatementNode(ASTExpressionNode* printStatement);
    ~ASTPrintStatementNode();

    void Accept(Visitor* v);

private:
    ASTExpressionNode* printStatement;

public:

};
