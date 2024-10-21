//
// Created by aidan on 06/05/2019.
//

#include "ASTFunctionDeclarationNode.h"

ASTFunctionDeclarationNode::ASTFunctionDeclarationNode(const std::string &functionName,
                                                       std::vector<ASTParameterNode *> *params, const std::string &type,
                                                       ASTStatementNode *body) {
    this->functionName = functionName;
    this->params = params;
    this->type = type;
    this->body = body;
}

ASTFunctionDeclarationNode::~ASTFunctionDeclarationNode() = default;

void ASTFunctionDeclarationNode::Accept(Visitor *v) {
    v->visit(this);
}

const std::string &ASTFunctionDeclarationNode::getFunctionName() const {
    return functionName;
}

std::vector<ASTParameterNode *> *ASTFunctionDeclarationNode::getParams() const {
    return params;
}

const std::string &ASTFunctionDeclarationNode::getType() const {
    return type;
}

ASTStatementNode *ASTFunctionDeclarationNode::getBody() const {
    return body;
}
