//
// Created by aidan on 06/05/2019.
//

#include "ASTParameterNode.h"

ASTParameterNode::ASTParameterNode(const std::string &name, const std::string &type) {
    this->name = name;
    this->type = type;
}
ASTParameterNode::~ASTParameterNode() = default;

void ASTParameterNode::Accept(Visitor *v) {
    v->visit(this);
}

const std::string &ASTParameterNode::getName() const {
    return name;
}

const std::string &ASTParameterNode::getType() const {
    return type;
}
