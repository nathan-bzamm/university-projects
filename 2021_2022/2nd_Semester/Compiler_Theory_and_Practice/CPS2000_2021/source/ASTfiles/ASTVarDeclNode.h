//
// Created by aidan on 04/05/2019.
//



#include "AST.h"

class ASTVarDeclNode: public ASTStatementNode
{
public:
    ASTVarDeclNode(const std::string &name,const  std::string &type, ASTExpressionNode* value);
    ~ASTVarDeclNode() override;

    virtual void Accept(Visitor* v);

    const std::string &getVarName() const;

    const std::string &getType() const;

    ASTExpressionNode *getValue() const;

private:
    std::string varName;
    std::string type;
    ASTExpressionNode* value;
};
