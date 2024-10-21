//
// Created by aidan on 07/05/2019.
//



#include "AST.h"

class ASTForLoopNode: public ASTStatementNode{
private:
    ASTStatementNode* decleration;
    ASTExpressionNode* condition;
    ASTStatementNode* assignment;
    ASTStatementNode* body;

public:
    ASTForLoopNode(ASTStatementNode* decleration,
    ASTExpressionNode* condition,
    ASTStatementNode* assignment,
    ASTStatementNode* body);

    ~ASTForLoopNode();

    virtual void Accept(Visitor* v);

    ASTStatementNode *getDecleration() const;

    void setDecleration(ASTStatementNode *decleration);

    ASTExpressionNode *getCondition() const;

    void setCondition(ASTExpressionNode *condition);

    ASTStatementNode *getAssignment() const;

    void setAssignment(ASTStatementNode *assignment);

    ASTStatementNode *getBody() const;

    void setBody(ASTStatementNode *body);
};
