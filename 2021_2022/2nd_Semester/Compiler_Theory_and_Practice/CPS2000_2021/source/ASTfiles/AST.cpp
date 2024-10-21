//
// Created by aidan on 30/04/2019.
//

#include "AST.h"

ASTNode::ASTNode() = default;

ASTNode::~ASTNode() = default;

ASTStatementNode::ASTStatementNode() = default;

ASTStatementNode::~ASTStatementNode() = default;

ASTExpressionNode::ASTExpressionNode() = default;

ASTExpressionNode::~ASTExpressionNode() = default;

void ASTExpressionNode::Accept(Visitor* v) {
    v->visit(this);
}

void ASTStatementNode::Accept(Visitor* v) {
    v->visit(this);
}