//
// Created by aidan on 10/05/2019.
//

#include "AST.h"

class ASTReturnStatementNode: public ASTExpressionNode{
public:
    ASTExpressionNode *getReturnExp() const;
    ASTReturnStatementNode(ASTExpressionNode* returnExp);
    ~ASTReturnStatementNode();

    virtual void Accept(Visitor* v);

private:
    ASTExpressionNode* returnExp;
};
