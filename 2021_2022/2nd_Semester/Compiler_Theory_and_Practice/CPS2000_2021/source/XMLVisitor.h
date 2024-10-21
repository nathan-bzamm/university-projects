//
// Created by aidan on 08/05/2019.
//

#include <string>
#include "Visitor.h"

class XMLVisitor: public Visitor{
private:
    int indents;

    //To prevent repetition of code
    void appendIndentsTo(std::string &xmlStatement);
    void increaseIndents();
    void decreaseIndents();
//    void outputBlockNodeXML(ASTStatementNode* node);

public:
    XMLVisitor();
    void visit(ASTPrintStatementNode* node) override;
    void visit(ASTReturnStatementNode* node) override;
    void visit(ASTStatementNode* node) override;
    void visit(ASTAssignmentStatementNode* node) override;
    void visit(ASTBlockStatementNode* node) override;
    void visit(ASTBinaryOpNode* node) override;
    void visit(ASTExpressionNode* node) override;
    void visit(ASTForLoopNode* node) override;
    void visit(ASTFunctionCallNode* node) override;
    void visit(ASTFunctionDeclarationNode* node) override;
    void visit(ASTIfStatementNode* node) override;
    void visit(ASTLiteralNode* node) override;
    void visit(ASTParameterNode* node) override;
    void visit(ASTProgramNode* node) override;
    void visit(ASTUnaryNode* node) override;
    void visit(ASTVarDeclNode* node) override;
    void visit(ASTVariableIdentifierNode* node) override;
};
