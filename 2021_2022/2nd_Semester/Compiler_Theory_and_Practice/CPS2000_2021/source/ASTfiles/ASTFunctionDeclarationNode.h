//
// Created by aidan on 06/05/2019.
//



#include "AST.h"
#include "ASTParameterNode.h"

class ASTFunctionDeclarationNode: public ASTStatementNode{
private:
    std::string functionName;
    std::vector<ASTParameterNode*>* params;
    std::string type;
    ASTStatementNode* body;

public:
    ASTFunctionDeclarationNode(const std::string &functionName, std::vector<ASTParameterNode*>* params,
            const std::string &type, ASTStatementNode* body);
    ~ASTFunctionDeclarationNode() override;

    virtual void Accept(Visitor* v);

    const std::string &getFunctionName() const;

    std::vector<ASTParameterNode *> *getParams() const;

    const std::string &getType() const;

    ASTStatementNode *getBody() const;
};


