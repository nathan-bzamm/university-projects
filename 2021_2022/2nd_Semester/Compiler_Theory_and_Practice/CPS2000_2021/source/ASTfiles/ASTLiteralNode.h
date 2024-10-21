//
// Created by aidan on 05/05/2019.
//



#include "AST.h"

class ASTLiteralNode: public ASTExpressionNode{
private:
    std::string type; // not using TOK for type to prevent importing Token.h
    float value;

public:
    ASTLiteralNode(const std::string &type, float val);
    ~ASTLiteralNode();

    virtual void Accept(Visitor* v);

    const std::string &getType() const;

    float getValue() const;
};