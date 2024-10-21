//
// Created by aidan on 06/05/2019.
//

#include "ASTAssignmentStatementNode.h"

ASTAssignmentStatementNode::ASTAssignmentStatementNode(const std::string &idName, ASTExpressionNode *result) {
    this->idName = idName;
    this->result = result;
}

ASTAssignmentStatementNode::~ASTAssignmentStatementNode() = default;

void ASTAssignmentStatementNode::Accept(Visitor *v) {
    v->visit(this);
}

const std::string &ASTAssignmentStatementNode::getIdName() const {
    return idName;
}

void ASTAssignmentStatementNode::setIdName(const std::string &idName) {
    ASTAssignmentStatementNode::idName = idName;
}

ASTExpressionNode *ASTAssignmentStatementNode::getResult() const {
    return result;
}

void ASTAssignmentStatementNode::setResult(ASTExpressionNode *result) {
    ASTAssignmentStatementNode::result = result;
}
