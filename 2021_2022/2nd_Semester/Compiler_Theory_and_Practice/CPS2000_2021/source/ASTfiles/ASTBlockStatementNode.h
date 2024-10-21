//
// Created by aidan on 09/05/2019.
//

#include "AST.h"

class ASTBlockStatementNode: public ASTStatementNode{
public:
    std::vector<ASTStatementNode *> *getStatements() const;

    ASTBlockStatementNode(std::vector<ASTStatementNode*>* statements);
    ~ASTBlockStatementNode();

    void Accept(Visitor *v) override;

private:
    std::vector<ASTStatementNode*>* statements;


public:

};
