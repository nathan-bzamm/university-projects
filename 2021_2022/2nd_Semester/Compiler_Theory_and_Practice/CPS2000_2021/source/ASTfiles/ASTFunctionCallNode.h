//
// Created by aidan on 05/05/2019.
//



#include "AST.h"

class ASTFunctionCallNode: public ASTExpressionNode{
private:
    std::string funcName;
    std::vector<ASTExpressionNode*>* params;

public:
    ASTFunctionCallNode(const std::string &funcName, std::vector<ASTExpressionNode*>* params);
    ~ASTFunctionCallNode();

    virtual void Accept(Visitor* v);

    const std::string &getFuncName() const;

    void setFuncName(const std::string &funcName);

    std::vector<ASTExpressionNode *> *getParams() const;

    void setParams(std::vector<ASTExpressionNode *> *params);
};
