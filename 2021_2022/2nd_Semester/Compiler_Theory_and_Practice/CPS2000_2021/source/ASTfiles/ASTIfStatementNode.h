//
// Created by aidan on 06/05/2019.
//



#include "AST.h"

#ifndef CPS2000_COMPILERS_ASSIGNMENT_ASTIFSTATEMENTNODE_H
#define CPS2000_COMPILERS_ASSIGNMENT_ASTIFSTATEMENTNODE_H

#endif //CPS2000_COMPILERS_ASSIGNMENT_ASTIFSTATEMENTNODE_H

class ASTIfStatementNode: public ASTStatementNode{
private:
    ASTExpressionNode* condition;
    ASTStatementNode* block;
    ASTStatementNode* elseBlock;

public:
    ASTIfStatementNode(ASTExpressionNode* cond, ASTStatementNode* block, ASTStatementNode* elseBlock);
    ~ASTIfStatementNode() override;

    virtual void Accept(Visitor* v);

    ASTExpressionNode *getCondition() const;

    ASTStatementNode *getBlock() const;

    ASTStatementNode *getElseBlock() const;
};