//
// Created by aidan on 07/05/2019.
//

#include "ASTForLoopNode.h"

ASTForLoopNode::ASTForLoopNode(ASTStatementNode *decleration, ASTExpressionNode *condition,
                               ASTStatementNode *assignment, ASTStatementNode *body) {
    this->decleration = decleration;
    this->condition = condition;
    this->assignment = assignment;
    this->body = body;
}

ASTForLoopNode::~ASTForLoopNode() = default;

void ASTForLoopNode::Accept(Visitor *v) {
    v->visit(this);
}

ASTStatementNode *ASTForLoopNode::getDecleration() const {
    return decleration;
}

void ASTForLoopNode::setDecleration(ASTStatementNode *decleration) {
    ASTForLoopNode::decleration = decleration;
}

ASTExpressionNode *ASTForLoopNode::getCondition() const {
    return condition;
}

void ASTForLoopNode::setCondition(ASTExpressionNode *condition) {
    ASTForLoopNode::condition = condition;
}

ASTStatementNode *ASTForLoopNode::getAssignment() const {
    return assignment;
}

void ASTForLoopNode::setAssignment(ASTStatementNode *assignment) {
    ASTForLoopNode::assignment = assignment;
}

ASTStatementNode *ASTForLoopNode::getBody() const {
    return body;
}

void ASTForLoopNode::setBody(ASTStatementNode *body) {
    ASTForLoopNode::body = body;
}
