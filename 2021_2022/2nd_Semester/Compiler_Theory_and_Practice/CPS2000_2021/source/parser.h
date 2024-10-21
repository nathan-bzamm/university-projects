//
// Created by aidan on 30/04/2019.
//
#pragma once

#ifndef CPS2000_COMPILERS_ASSIGNMENT_PARSER_H
#define CPS2000_COMPILERS_ASSIGNMENT_PARSER_H

#include "lexer.h"
#include "Token.h"
#include "ASTfiles/AST.h"
#include "ASTfiles/ASTVarDeclNode.h"
#include "ASTfiles/ASTLiteralNode.h"

#include "ASTfiles/ASTBinaryOpNode.h"
#include "ASTfiles/ASTUnaryNode.h"

#include "ASTfiles/ASTFunctionCallNode.h"
#include "ASTfiles/ASTVariableIdentifierNode.h"
#include "ASTfiles/ASTIfStatementNode.h"
#include "ASTfiles/ASTFunctionDeclarationNode.h"
#include "ASTfiles/ASTAssignmentStatementNode.h"
#include "ASTfiles/ASTForLoopNode.h"
#include "ASTfiles/ASTProgramNode.h"
#include "ASTfiles/ASTBlockStatementNode.h"
#include "ASTfiles/ASTReturnStatementNode.h"
#include "ASTfiles/ASTPrintStatementNode.h"
#include "ASTfiles/ASTProgramNode.h"



class Parser{
public:
    Parser(Lexer* lex);
    ASTNode* parse();

private:
    Lexer* lexer;
    Token* CurrentToken;
    ASTProgramNode* Root; //TODO: CHANGE THIS TO PROGRAM NODE

    void Error(const std::string &errMsg,int lineNumber);
    ASTVarDeclNode* parseVarDeclaration();
    ASTExpressionNode* parseExpression();
    ASTExpressionNode* parseTerm();
    ASTExpressionNode* parseSimpleExpression();
    ASTExpressionNode* parseIdentifierExpression();
    ASTStatementNode* parseStatement();
    ASTStatementNode* parseIfStatement();
    ASTStatementNode* parseForLoopStatement();
    ASTStatementNode* parseFunctionDeclaration();
    ASTStatementNode* parseAssignmentStatement();
    ASTStatementNode* parseBlock();


    bool isRelop(TOK t);
    bool isMultOp(TOK t);
    bool isAddOp(TOK t);
};

#endif //CPS2000_COMPILERS_ASSIGNMENT_PARSER_H
