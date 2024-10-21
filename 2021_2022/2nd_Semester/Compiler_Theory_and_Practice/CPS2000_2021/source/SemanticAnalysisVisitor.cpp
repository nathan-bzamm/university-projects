//
// Created by aidan on 13/05/2019.
//

#include "SemanticAnalysisVisitor.h"
#include <iostream>

#include "ASTfiles/AST.h"
#include "ASTfiles/ASTAssignmentStatementNode.h"
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
#include "ASTfiles/ASTPrintStatementNode.h"
#include "ASTfiles/ASTReturnStatementNode.h"

using namespace std;

SemanticAnalysisVisitor::SemanticAnalysisVisitor() {
    symbolTable = new vector<map<string, tableEntry*>*>;
    dataTypeScopeStack = new vector<dataType>;
}

SemanticAnalysisVisitor::~SemanticAnalysisVisitor() = default;

void SemanticAnalysisVisitor::STPush(){
    auto newScope = new map<string, tableEntry*>;
    symbolTable->push_back(newScope);
}

void SemanticAnalysisVisitor::STPop() {
    if(!symbolTable->empty()){
        delete symbolTable->back();
        symbolTable->pop_back(); //delete map on top of stack
    }
}

SemanticAnalysisVisitor::tableEntry * SemanticAnalysisVisitor::STLookup(std::string name) {
    map<string, tableEntry*>* currentScope;

    if(symbolTable->empty()) return nullptr;

    for(unsigned int i = symbolTable->size(); i>0; --i) {
        currentScope = symbolTable->at(i-1);
        auto tableEntry = currentScope->find(name);
        if (tableEntry != currentScope->end()) {
            //if found, return entry
            return tableEntry->second;
        }
    }
    //if this point is reached, variable/function name doesnt exist in any scope
    return nullptr;
}

void SemanticAnalysisVisitor::STInsert(const string &name,dataType dType, idType entryType, std::vector<ASTParameterNode*>* params)
{
    auto currentScope = symbolTable->back();
    if(currentScope->find(name) != currentScope->end()){
        //if entry is found in map already, throw error
        string err = "A Function/Variable called \'" + name +"\' already exists in the same scope!";
        reportError(err);
    }

    //Else add it to current map
    auto newEntry = new tableEntry(dType, entryType, params);
    currentScope->insert(pair<string, tableEntry*>(name, newEntry));
}

void SemanticAnalysisVisitor::reportError(const std::string &errorMsg) {
    cout<<"Compilation Error!"<<endl;
    cerr<<errorMsg<<endl;
    exit(EXIT_FAILURE);
}

void SemanticAnalysisVisitor::dtStackAdd(SemanticAnalysisVisitor::dataType t) {
//    cout<<"PUSHING ON STACK: "<<dataTypeToString(t)<<endl;
    dataTypeScopeStack->push_back(t);
}

SemanticAnalysisVisitor::dataType SemanticAnalysisVisitor::dtStackPop() {
    if(!dataTypeScopeStack->empty()) {
        auto t = dataTypeScopeStack->back();
//        cout<<"POPPING FROM STACK: "<<dataTypeToString(t)<<endl;
        dataTypeScopeStack->pop_back();
        return t;
    }
}

SemanticAnalysisVisitor::dataType SemanticAnalysisVisitor::dataTypeToEnum(const std::string &type) {
    if(type == "TOK_INTEGERLIT" || type == "int") return INT;
    else if (type == "TOK_FLOATLIT" || type == "float") return FLT;
    else if (type == "TOK_BOOLEANLIT" || type =="bool") return BOOL;
    else{cerr<<"whoooooaaah wrong ting bro: "<<type<<endl;}
}

string SemanticAnalysisVisitor::dataTypeToString(SemanticAnalysisVisitor::dataType d) {
    switch(d){
        case INT: return "int";
        case FLT: return "float";
        case BOOL: return "bool";
        default: reportError("Incorrect type to convert");
    }
}

bool SemanticAnalysisVisitor::isRelop(const std::string &op) {
    return op=="<" || op==">" || op=="!=" || op=="<=" || op==">=" || op=="==";
}

void SemanticAnalysisVisitor::visit(ASTPrintStatementNode *node) {
    node->getPrintStatement()->Accept(this);
    dtStackPop(); // discard type of expression as no need to compare
}

void SemanticAnalysisVisitor::visit(ASTVarDeclNode *node) {
    dataType type = dataTypeToEnum(node->getType());
    STInsert(node->getVarName(), type, VAR);
    //now we just let expression do its job and the value on TOS should be the data type of the expression
    node->getValue()->Accept(this);
    dataType resultType = dtStackPop();
    if(resultType != type){
        string err = "Variable " + node->getVarName()+ " of type \'" +dataTypeToString(type)+ "\' cannot be assigned type \'" +dataTypeToString(resultType)+"\'!";
        reportError(err);
    }
}

void SemanticAnalysisVisitor::visit(ASTBinaryOpNode* node) {
    node->getLHS()->Accept(this);
    dataType lhsType = dtStackPop();
    node->getRHS()->Accept(this);
    dataType rhsType = dtStackPop();

    if(lhsType == rhsType)dtStackAdd(lhsType);//doesnt really matter which side to push
    else{
        string err = "Expression has different types! (\'" +dataTypeToString(lhsType)+"\' and \'"+dataTypeToString(rhsType)+"\')";
        reportError(err);
    }

    if((node->getOp()=="and" || node->getOp()=="or")&& lhsType!=BOOL){
        string err = "\'" + node->getOp()+"\' is a boolean operator but is given type \'"+dataTypeToString(lhsType)+"\'";
        reportError(err);
    }

    if(isRelop(node->getOp())){
        dtStackPop(); //pop off the previous lhsType and push a BOOL
        dtStackAdd(BOOL); //a relational operator returns only true or false
    }
}

void SemanticAnalysisVisitor::visit(ASTLiteralNode* node) {
    //Just push type of literal to node. Other functions higher up will check types
    dtStackAdd(dataTypeToEnum(node->getType()));
}

void SemanticAnalysisVisitor::visit(ASTAssignmentStatementNode* node){
    tableEntry* lookupID = STLookup(node->getIdName());
    //Only variables allowed in assignment
    if(lookupID == nullptr || lookupID->entryType == FUNC){
        string err = "Variable " + node->getIdName()+" doesn't exist or is inaccessible from current scope!";
        reportError(err);
    }
    else {
        node->getResult()->Accept(this);
        dataType resultType = dtStackPop();
        if (lookupID->dType != resultType){
            string err = "Variable " + node->getIdName()+ " of type \'" +dataTypeToString(lookupID->dType)+ "\' cannot be assigned type \'" +dataTypeToString(resultType)+"\'!";
            reportError(err);
        }
    }
}

void SemanticAnalysisVisitor::visit(ASTFunctionDeclarationNode* node) {
    STInsert(node->getFunctionName(),dataTypeToEnum(node->getType()),FUNC, node->getParams());

    //Adding function data type to stack for return statement
    dtStackAdd(dataTypeToEnum(node->getType()));

    STPush();

    if(node->getParams()!= nullptr){
        for(auto params: *node->getParams()) {
            if (params != nullptr) {
                params->Accept(this);
            }
        }
    }

    node->getBody()->Accept(this);

    STPop();//destroy scope

}

void SemanticAnalysisVisitor::visit(ASTReturnStatementNode* node){
    dataType funcType = dtStackPop();

    node->getReturnExp()->Accept(this);

    dataType resultType = dtStackPop();
    if(funcType!=resultType){
        string err = "Function return type (\'"+dataTypeToString(funcType)+"\') "+ " mismatches that of expression (\'"+dataTypeToString(resultType)+"\')";
        reportError(err);
    }
}

void SemanticAnalysisVisitor::visit(ASTBlockStatementNode* node){
    for(auto statement: *node->getStatements()){
        if(statement != nullptr) {
            statement->Accept(this);
        }
    }
}

void SemanticAnalysisVisitor::visit(ASTFunctionCallNode* node){
    tableEntry* lookupID = STLookup(node->getFuncName());
    if(lookupID == nullptr || lookupID->entryType != FUNC){
        string err = "Function " + node->getFuncName()+" doesn't exist or is inaccessible from current scope!";
        reportError(err);
    }

    //make sure params mtch up
    if(lookupID->parameters!=nullptr && node->getParams()!= nullptr){
        int funcParamSize = lookupID->parameters->size();
        int funcCallParamSize = node->getParams()->size();

        if(funcParamSize > funcCallParamSize){
            string err = "Not enough arguments in function call: "+node->getFuncName();
            reportError(err);
        }
        else if(funcParamSize < funcCallParamSize){
            string err = "Too many arguments in function call: "+node->getFuncName();
            reportError(err);
        }

        unsigned int index = 0;
        for(auto param : *lookupID->parameters){
            if(param!=nullptr) {
                dataType parType = dataTypeToEnum(param->getType());
                node->getParams()->at(index)->Accept(this);
                dataType resultType = dtStackPop();
                if (parType != resultType) {
                    string err = "Parameter and argument types don't match function call: " + node->getFuncName();
                    reportError(err);
                }
                index++;
            }
        }
    }
    else{
        string err = "Not enough/Too many arguments in function call: "+node->getFuncName();
        reportError(err);
    }
    dtStackAdd(lookupID->dType); //add type on stack for higher functions to compare
}

void SemanticAnalysisVisitor::visit(ASTForLoopNode* node){
    STPush();//new scope

    if(node->getDecleration() != nullptr){
        node->getDecleration()->Accept(this);
    }

    node->getCondition()->Accept(this);
    dtStackPop(); //discard expression type since no checking needed

    if(node->getAssignment() != nullptr){
        node->getAssignment()->Accept(this);
    }

    node->getBody()->Accept(this);
    STPop();
}

void SemanticAnalysisVisitor::visit(ASTVariableIdentifierNode* node){
    tableEntry* lookup = STLookup(node->getName());
    if(lookup == nullptr){
        string err = "Variable " + node->getName()+" doesn't exist or is inaccessible from current scope!";
        reportError(err);
    }
    dtStackAdd(lookup->dType);
}

void SemanticAnalysisVisitor::visit(ASTIfStatementNode* node){
    node->getCondition()->Accept(this);
    dtStackPop(); //discard result from expression since no need for type checking outside of it
    STPush();
    node->getBlock()->Accept(this);
    STPop();
    if(node->getElseBlock() != nullptr){
        STPush();
        node->getElseBlock()->Accept(this);
        STPop();
    }
}

void SemanticAnalysisVisitor::visit(ASTParameterNode* node){
    STInsert(node->getName(),dataTypeToEnum(node->getType()),VAR,nullptr);
}

void SemanticAnalysisVisitor::visit(ASTUnaryNode* node){
    const string &op = node->getType();
    node->getExp()->Accept(this);

    dataType resultType = dtStackPop();
    if(op == "-" && resultType == BOOL){
        string err = "Incompatible unary types! \'"+op+"\' and \'"+dataTypeToString(resultType)+"\'";
        reportError(err);
    }
    else if(op=="not" && resultType !=BOOL){
        string err = "Incompatible unary types! \'"+op+"\' and \'"+dataTypeToString(resultType)+"\'";
        reportError(err);
    }

    dtStackAdd(resultType); //adding the result back on stack for higher functions to use
}

void SemanticAnalysisVisitor::visit(ASTProgramNode* node){
    STPush(); //global scope
    for(auto statement: *node->getProgram()){
        if(statement != nullptr) {
            statement->Accept(this);
        }
    }
    STPop();
}

void SemanticAnalysisVisitor::visit(ASTExpressionNode* node) {/*Do nothing*/}

void SemanticAnalysisVisitor::visit(ASTStatementNode* node){/*Do nothing*/}
