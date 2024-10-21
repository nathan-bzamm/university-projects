//
// Created by aidan on 13/05/2019.
//
#pragma once

#include <map>
#include <vector>
#include "Visitor.h"



class SemanticAnalysisVisitor:public Visitor{
private:
    enum idType{VAR, FUNC};
    enum dataType{INT,FLT,BOOL};

    struct tableEntry{
        dataType dType; //
        idType entryType;
        std::vector<ASTParameterNode*>* parameters; //holds the name of the parameters to be checked in the symble table

        tableEntry(dataType d, idType t, std::vector<ASTParameterNode*>* pars){
            dType = d;
            entryType = t;
            parameters = pars;
        }
    };//tableEntry;

    std::vector<std::map<std::string, tableEntry*>*>* symbolTable;

    std::vector<dataType>* dataTypeScopeStack; //hold the top most data type for semantic analysis

    void STPush(); // pushes a new map on the stack (new scope)
    void STInsert(const std::string &name,dataType dType, idType entryType, std::vector<ASTParameterNode*>* params= nullptr); //enters a variable in the top most map
    tableEntry* STLookup(std::string name);
    void STPop(); //pops out the current map/scope

    void reportError(const std::string &errorMsg);
    void dtStackAdd(dataType t);
    dataType dtStackPop();

    bool isRelop(const std::string &op);

    dataType dataTypeToEnum(const std::string &type);
    std::string dataTypeToString(dataType d);

public:
    SemanticAnalysisVisitor();
    ~SemanticAnalysisVisitor();
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
