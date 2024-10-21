//
// Created by aidan on 05/05/2019.
//


#include "AST.h"


class ASTBinaryOpNode: public ASTExpressionNode{
public:
    ASTBinaryOpNode(ASTExpressionNode* LHS,const std::string &op, ASTExpressionNode* RHS);
    ~ASTBinaryOpNode();

    virtual void Accept(Visitor* v);

    ASTExpressionNode *getLHS() const;

    void setLHS(ASTExpressionNode *LHS);

    const std::string &getOp() const;

    void setOp(const std::string &op);

    ASTExpressionNode *getRHS() const;

    void setRHS(ASTExpressionNode *RHS);

private:
    ASTExpressionNode* LHS;
    std::string op;
    ASTExpressionNode* RHS;
};