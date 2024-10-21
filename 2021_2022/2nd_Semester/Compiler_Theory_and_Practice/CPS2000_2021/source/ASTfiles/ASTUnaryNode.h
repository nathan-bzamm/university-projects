//
// Created by aidan on 05/05/2019.
//

#include "AST.h"

class ASTUnaryNode: public ASTExpressionNode{
private:
    ASTExpressionNode* exp;
    std::string type;

public:
    ASTUnaryNode(const std::string &type,ASTExpressionNode* expression);
    ~ASTUnaryNode();

    virtual void Accept(Visitor* v);

    ASTExpressionNode *getExp() const;

    const std::string &getType() const;
};
