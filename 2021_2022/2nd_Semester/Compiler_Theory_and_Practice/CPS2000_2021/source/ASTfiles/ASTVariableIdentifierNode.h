//
// Created by aidan on 05/05/2019.
//



#include "AST.h"

class ASTVariableIdentifierNode: public ASTExpressionNode{
private:
    std::string name;

public:
    ASTVariableIdentifierNode(const std::string &name);
    ~ASTVariableIdentifierNode();

    virtual void Accept(Visitor* v);

    const std::string &getName() const;
};
