//
// Created by aidan on 04/05/2019.
//

#include "ASTVarDeclNode.h"

ASTVarDeclNode::ASTVarDeclNode(const std::string &name,const std::string &type, ASTExpressionNode *value){
    varName = name;
    this->type = type;
    this->value = value;
}

ASTVarDeclNode::~ASTVarDeclNode() = default;

void ASTVarDeclNode::Accept(Visitor *v) {
    v->visit(this);
}

const std::string &ASTVarDeclNode::getVarName() const {
    return varName;
}

const std::string &ASTVarDeclNode::getType() const {
    return type;
}

ASTExpressionNode *ASTVarDeclNode::getValue() const {
    return value;
}
