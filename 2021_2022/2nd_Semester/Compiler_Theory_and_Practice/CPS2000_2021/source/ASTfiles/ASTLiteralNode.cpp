//
// Created by aidan on 05/05/2019.
//

#include "ASTLiteralNode.h"

ASTLiteralNode::ASTLiteralNode(const std::string &type, float val) {
    this->type = type;
    this->value = val;
}

ASTLiteralNode::~ASTLiteralNode() = default;

void ASTLiteralNode::Accept(Visitor *v) {
    v->visit(this);
}

const std::string &ASTLiteralNode::getType() const {
    return type;
}

float ASTLiteralNode::getValue() const {
    return value;
}
