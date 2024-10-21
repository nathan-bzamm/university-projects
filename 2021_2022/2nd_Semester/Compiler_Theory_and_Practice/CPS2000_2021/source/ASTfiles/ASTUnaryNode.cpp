//
// Created by aidan on 05/05/2019.
//

#include "ASTUnaryNode.h"

ASTUnaryNode::ASTUnaryNode(const std::string &type, ASTExpressionNode* expression) {
    this->type = type;
    exp = expression;
}

ASTUnaryNode::~ASTUnaryNode() = default;

void ASTUnaryNode::Accept(Visitor *v) {
    v->visit(this);
}

ASTExpressionNode *ASTUnaryNode::getExp() const {
    return exp;
}

const std::string &ASTUnaryNode::getType() const {
    return type;
}
