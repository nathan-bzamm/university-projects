//
// Created by aidan on 07/05/2019.
//

#include "ASTProgramNode.h"

ASTProgramNode::ASTProgramNode(std::vector<ASTStatementNode*>* programVector){
        this->program = programVector;
}

ASTProgramNode::~ASTProgramNode() = default;

void ASTProgramNode::Accept(Visitor *v) {
    v->visit(this);
}

std::vector<ASTStatementNode *> *ASTProgramNode::getProgram() const {
    return program;
}
