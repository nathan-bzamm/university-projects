//
// Created by aidan on 10/05/2019.
//

#include "ASTPrintStatementNode.h"

ASTExpressionNode *ASTPrintStatementNode::getPrintStatement() const {
    return printStatement;
}

ASTPrintStatementNode::ASTPrintStatementNode(ASTExpressionNode *printStatement) {
    this->printStatement = printStatement;
}

ASTPrintStatementNode::~ASTPrintStatementNode() = default;

void ASTPrintStatementNode::Accept(Visitor *v) {
    v->visit(this);
}
