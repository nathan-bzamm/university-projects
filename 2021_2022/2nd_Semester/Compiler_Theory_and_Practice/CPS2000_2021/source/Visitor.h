//
// Created by aidan on 30/04/2019.
//

#pragma once



class ASTNode;
class ASTStatementNode;
class ASTAssignmentStatementNode;
class ASTBinaryOpNode;
class ASTExpressionNode;
class ASTForLoopNode;
class ASTFunctionCallNode;
class ASTFunctionDeclarationNode;
class ASTIfStatementNode;
class ASTLiteralNode;
class ASTParameterNode;
class ASTProgramNode;
class ASTUnaryNode;
class ASTVarDeclNode;
class ASTVariableIdentifierNode;
class ASTBlockStatementNode;
class ASTReturnStatementNode;
class ASTPrintStatementNode;


class Visitor{
public:
    //virtual void ASTNode(ASTNode*) = 0;
    virtual void visit(ASTReturnStatementNode* ) = 0;
    virtual void visit(ASTPrintStatementNode* ) = 0;
    virtual void visit(ASTStatementNode*) = 0;
    virtual void visit(ASTAssignmentStatementNode*) = 0;
    virtual void visit(ASTBlockStatementNode*) = 0;
    virtual void visit(ASTBinaryOpNode*) = 0;
    virtual void visit(ASTExpressionNode*) = 0;
    virtual void visit(ASTForLoopNode*) = 0;
    virtual void visit(ASTFunctionCallNode*) = 0;
    virtual void visit(ASTFunctionDeclarationNode*) = 0;
    virtual void visit(ASTIfStatementNode*) = 0;
    virtual void visit(ASTLiteralNode*) = 0;
    virtual void visit(ASTParameterNode*) = 0;
    virtual void visit(ASTProgramNode*) = 0;
    virtual void visit(ASTUnaryNode*) = 0;
    virtual void visit(ASTVarDeclNode*) = 0;
    virtual void visit(ASTVariableIdentifierNode*) = 0;
};
