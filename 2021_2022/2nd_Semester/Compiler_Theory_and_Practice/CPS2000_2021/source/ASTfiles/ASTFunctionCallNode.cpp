//
// Created by aidan on 05/05/2019.
//

#include "ASTFunctionCallNode.h"

ASTFunctionCallNode::ASTFunctionCallNode(const std::string &funcName, std::vector<ASTExpressionNode *> *params) {
    this->funcName = funcName;
    this->params = params;
}

ASTFunctionCallNode::~ASTFunctionCallNode() = default;

void ASTFunctionCallNode::Accept(Visitor *v) {
    v->visit(this);
}

const std::string &ASTFunctionCallNode::getFuncName() const {
    return funcName;
}

void ASTFunctionCallNode::setFuncName(const std::string &funcName) {
    ASTFunctionCallNode::funcName = funcName;
}

std::vector<ASTExpressionNode *> *ASTFunctionCallNode::getParams() const {
    return params;
}

void ASTFunctionCallNode::setParams(std::vector<ASTExpressionNode *> *params) {
    ASTFunctionCallNode::params = params;
}
