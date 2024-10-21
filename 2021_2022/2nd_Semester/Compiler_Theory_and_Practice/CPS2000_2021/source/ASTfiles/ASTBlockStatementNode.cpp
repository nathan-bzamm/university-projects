//
// Created by aidan on 09/05/2019.
//

#include "ASTBlockStatementNode.h"

ASTBlockStatementNode::ASTBlockStatementNode(std::vector<ASTStatementNode *> *statements) {
    this->statements = statements;
}

ASTBlockStatementNode::~ASTBlockStatementNode() = default;

std::vector<ASTStatementNode *> *ASTBlockStatementNode::getStatements() const {
    return statements;
}

void ASTBlockStatementNode::Accept(Visitor *v) {
    v->visit(this);
}