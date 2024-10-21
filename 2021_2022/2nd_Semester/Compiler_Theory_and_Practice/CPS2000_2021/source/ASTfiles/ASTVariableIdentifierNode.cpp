//
// Created by aidan on 05/05/2019.
//

#include "ASTVariableIdentifierNode.h"

ASTVariableIdentifierNode::ASTVariableIdentifierNode(const std::string &name) {
    this->name = name;
}

ASTVariableIdentifierNode::~ASTVariableIdentifierNode() = default;

void ASTVariableIdentifierNode::Accept(Visitor *v) {
    v->visit(this);
}

const std::string &ASTVariableIdentifierNode::getName() const {
    return name;
}
