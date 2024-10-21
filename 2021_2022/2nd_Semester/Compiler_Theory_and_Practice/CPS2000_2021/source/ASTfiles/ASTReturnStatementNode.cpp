//
// Created by aidan on 10/05/2019.
//

#include "ASTReturnStatementNode.h"

ASTExpressionNode *ASTReturnStatementNode::getReturnExp() const {
    return returnExp;
}

ASTReturnStatementNode::ASTReturnStatementNode(ASTExpressionNode* returnExp) {
    this->returnExp = returnExp;
}

ASTReturnStatementNode::~ASTReturnStatementNode() = default;

void ASTReturnStatementNode::Accept(Visitor* v){
    v->visit(this);
}