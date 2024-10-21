//
// Created by aidan on 17/05/2019.
//

#include "InterpreterVisitor.h"

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

InterpreterVisitor::InterpreterVisitor() {
    symbolTable = new  vector<map<std::string, tableEntry*>*>;
    resultStack = new std::vector<float>;
}

InterpreterVisitor::~InterpreterVisitor() = default;

InterpreterVisitor::dataType InterpreterVisitor::dataTypeToEnum(const std::string &type) {
    if(type == "TOK_INTEGERLIT" || type == "int") return INT;
    else if (type == "TOK_FLOATLIT" || type == "float") return FLT;
    else if (type == "TOK_BOOLEANLIT" || type =="bool") return BOOL;
    else{cerr<<"whoooooaaah wrong ting bro: "<<type<<endl;}
}

void InterpreterVisitor::STPush(){
    auto newScope = new map<string, tableEntry*>;
    symbolTable->push_back(newScope);
}

void InterpreterVisitor::STPop() {
    if(!symbolTable->empty()){
        delete symbolTable->back();
        symbolTable->pop_back(); //delete map on top of stack
    }
}

InterpreterVisitor::tableEntry * InterpreterVisitor::STLookup(std::string name) {
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

void InterpreterVisitor::STInsert(const std::string &name,dataType dType, idType entryType,float val,
        ASTStatementNode* body,std::vector<ASTParameterNode*>* params)
{
    auto currentScope = symbolTable->back();
    if(currentScope->find(name) != currentScope->end()){
        //if entry is found in map already, throw error
        string err = "A Function/Variable called \'" + name +"\' already exists in the same scope!";
//        reportError(err);
    }

    //Else add it to current map
    auto newEntry = new tableEntry(dType, entryType,val, body, params);
    currentScope->insert(pair<string, tableEntry*>(name, newEntry));
}

void InterpreterVisitor::STUpdate(const std::string &id, float newVal) {
    tableEntry* entryToUpdate = STLookup(id);
    entryToUpdate->value = newVal;
}

void InterpreterVisitor::resultStackPush(float r){
    resultStack->push_back(r);
}

float InterpreterVisitor::resultStackPop(){
    if(!resultStack->empty()) {
        auto t = resultStack->back();
        resultStack->pop_back();
        return t;
    }
}


void InterpreterVisitor::visit(ASTVarDeclNode* node){
    //Just let expression do its job and the value on TOS should be the data type of the expression
    node->getValue()->Accept(this);
    float result = resultStackPop();
    STInsert(node->getVarName(), dataTypeToEnum(node->getType()), VAR, result);
}

void InterpreterVisitor::visit(ASTBinaryOpNode* node){
    node->getLHS()->Accept(this);
    float lhs = resultStackPop();
    node->getRHS()->Accept(this);
    float rhs = resultStackPop();

    float expressionResult= 0;
    const string &op = node->getOp();

    /*First character could be op or could be relop ( ex. '+' or '<')
      Hacky optimisation cause I'm a bad planner*/
    switch(op[0]){
        case '+': expressionResult = lhs + rhs;break;
        case '-': expressionResult = lhs - rhs;break;
        case '*': expressionResult = lhs * rhs;break;
        case '/': expressionResult = lhs / rhs;break;
        case 'a': expressionResult =(float)((bool)lhs && (bool)rhs);break; // and
        case 'o': expressionResult =(float)((bool)lhs || (bool)rhs);break; // or
        case '<':{
            if(op.length()>1 && op[1] == '=')
                expressionResult =(float)(lhs <= rhs);
            else
                expressionResult =(float)(lhs < rhs);
        };break;
        case '>':{
            if(op.length()>1 && op[1] == '=')
                expressionResult =(float)(lhs >= rhs);
            else
                expressionResult =(float)(lhs > rhs);
        };break;
        case '=': expressionResult =(float)(lhs == rhs);break; // ==
        case '!': expressionResult =(float)(lhs != rhs);break; // !=
        default: cerr<<"Unexpected operator interpreting error"<<endl;
    }
    resultStackPush(expressionResult);
}

void InterpreterVisitor::visit(ASTAssignmentStatementNode* node){
    tableEntry* lookupID = STLookup(node->getIdName());

    //Only variables allowed in assignment
    if(lookupID == nullptr || lookupID->entryType == FUNC){
        string err = "Variable " + node->getIdName()+" doesn't exist or is inaccessible from current scope!";
//        reportError(err);
    }
    else {
        node->getResult()->Accept(this);
        float result = resultStackPop();
        STUpdate(node->getIdName(), result);
    }
}

void InterpreterVisitor::visit(ASTUnaryNode* node){
    node->getExp()->Accept(this);

    const string &op = node->getType();
    float result = resultStackPop();
    float unaryResult = 0;

    if(op == "-") unaryResult = -result;
    else unaryResult = (float) !(bool)result;

    resultStackPush(unaryResult);
}

void InterpreterVisitor::visit(ASTProgramNode* node){
    STPush(); //global scope
    for(auto statement: *node->getProgram()){
        if(statement != nullptr) {
            statement->Accept(this);
        }
    }
    STPop();
}

void InterpreterVisitor::visit(ASTFunctionCallNode* node) {
    tableEntry *lookupID = STLookup(node->getFuncName());
    if (lookupID == nullptr || lookupID->entryType != FUNC) {
        string err = "Function " + node->getFuncName() + " doesn't exist or is inaccessible from current scope!";
//        reportError(err);
    }

    STPush();//create new scope for running function call

    //make sure params mtch up
    unsigned int index = 0;
    for (auto param : *lookupID->parameters) {
        if (param != nullptr) {
            node->getParams()->at(index)->Accept(this); //run expression
            param->Accept(this); //place result in param
            index++;
        }
    }

    lookupID->funcBody->Accept(this);
    STPop();
}

void InterpreterVisitor::visit(ASTIfStatementNode* node){
    node->getCondition()->Accept(this);
    if((bool)resultStackPop()) {
        STPush();
        node->getBlock()->Accept(this);
        STPop();
    }
    else if(node->getElseBlock() != nullptr){
        STPush();
        node->getElseBlock()->Accept(this);
        STPop();
    }
}

void InterpreterVisitor::visit(ASTForLoopNode* node){
    STPush();//new scope

    if(node->getDecleration() != nullptr){
        node->getDecleration()->Accept(this);
    }

    node->getCondition()->Accept(this);
    while((bool)resultStackPop()){ //while condition is true

        node->getBody()->Accept(this); //execute body

        //increment assignment if applicable
        if(node->getAssignment() != nullptr){
            node->getAssignment()->Accept(this);
        }
        node->getCondition()->Accept(this); //re check condition
    }
    STPop();
}

void InterpreterVisitor::visit(ASTPrintStatementNode *node) {
    node->getPrintStatement()->Accept(this);
    cout<<resultStackPop()<<endl;
}

void InterpreterVisitor::visit(ASTReturnStatementNode* node){
    node->getReturnExp()->Accept(this);
}

void InterpreterVisitor::visit(ASTParameterNode* node){
    STInsert(node->getName(),dataTypeToEnum(node->getType()),VAR, resultStackPop());
}

void InterpreterVisitor::visit(ASTFunctionDeclarationNode* node) {
    STInsert(node->getFunctionName(),dataTypeToEnum(node->getType()),FUNC, 0,node->getBody(), node->getParams());
}

void InterpreterVisitor::visit(ASTLiteralNode* node) {
    //Just push type of literal to node. Other functions higher up will check types
    resultStackPush(node->getValue());
}

void InterpreterVisitor::visit(ASTVariableIdentifierNode* node){
    tableEntry* lookup = STLookup(node->getName());
    if(lookup == nullptr){
        string err = "Variable " + node->getName()+" doesn't exist or is inaccessible from current scope!";
//        reportError(err);
    }
    resultStackPush(lookup->value);
}

void InterpreterVisitor::visit(ASTBlockStatementNode* node){
    for(auto statement: *node->getStatements()){
        if(statement != nullptr) {
            statement->Accept(this);
        }
    }
}

void InterpreterVisitor::visit(ASTExpressionNode* node) {/*Do nothing*/}

void InterpreterVisitor::visit(ASTStatementNode* node){/*Do nothing*/}