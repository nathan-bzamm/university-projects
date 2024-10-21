//
// Created by aidan on 06/05/2019.
//



#include <string>
#include "AST.h"

class ASTParameterNode:ASTExpressionNode{
private:
    std::string name;
    std::string type;

public:
    ASTParameterNode(const std::string &name, const std::string &type);
    ~ASTParameterNode();

    virtual void Accept(Visitor* v);

    const std::string &getName() const;

    const std::string &getType() const;
};
