//
// Created by aidan on 05/05/2019.
//

#include "ASTBinaryOpNode.h"

ASTBinaryOpNode::ASTBinaryOpNode(ASTExpressionNode* LHS,const std::string &op, ASTExpressionNode* RHS) {
    this->LHS = LHS;
    this->op = op;
    this->RHS = RHS;
}

ASTBinaryOpNode::~ASTBinaryOpNode()= default;

void ASTBinaryOpNode::Accept(Visitor *v) {
    v->visit(this);
}

ASTExpressionNode *ASTBinaryOpNode::getLHS() const {
    return LHS;
}

void ASTBinaryOpNode::setLHS(ASTExpressionNode *LHS) {
    ASTBinaryOpNode::LHS = LHS;
}

const std::string &ASTBinaryOpNode::getOp() const {
    return op;
}

void ASTBinaryOpNode::setOp(const std::string &op) {
    ASTBinaryOpNode::op = op;
}

ASTExpressionNode *ASTBinaryOpNode::getRHS() const {
    return RHS;
}

void ASTBinaryOpNode::setRHS(ASTExpressionNode *RHS) {
    ASTBinaryOpNode::RHS = RHS;
}
