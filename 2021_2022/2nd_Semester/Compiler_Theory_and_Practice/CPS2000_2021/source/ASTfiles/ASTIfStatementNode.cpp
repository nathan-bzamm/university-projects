//
// Created by aidan on 06/05/2019.
//

#include "ASTIfStatementNode.h"

ASTIfStatementNode::ASTIfStatementNode(ASTExpressionNode *cond, ASTStatementNode *block, ASTStatementNode *elseBlock) {
    this->condition = cond;
    this->block = block;
    this->elseBlock = elseBlock;
}

ASTIfStatementNode::~ASTIfStatementNode() = default;

void ASTIfStatementNode::Accept(Visitor *v) {
    v->visit(this);
}

ASTExpressionNode *ASTIfStatementNode::getCondition() const {
    return condition;
}

ASTStatementNode *ASTIfStatementNode::getBlock() const {
    return block;
}

ASTStatementNode *ASTIfStatementNode::getElseBlock() const {
    return elseBlock;
}
