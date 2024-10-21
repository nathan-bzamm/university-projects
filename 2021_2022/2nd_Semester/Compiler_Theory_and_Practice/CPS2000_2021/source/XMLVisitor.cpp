//
// Created by aidan on 08/05/2019.
//

#include "XMLVisitor.h"
#include "ASTfiles/ASTAssignmentStatementNode.h"
#include <iostream>

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
#include "ASTfiles/ASTPrintStatementNode.h"
#include "ASTfiles/ASTReturnStatementNode.h"

using namespace std;

XMLVisitor::XMLVisitor() {
    indents = 0;
}

// Appends a number of tabs to a string depending on the depth of the xml statement
void XMLVisitor::appendIndentsTo(std::string &xmlStatement) {
    for(int i=0;i<indents;i++){
        xmlStatement.append("\t");
    }
}

void XMLVisitor::visit(ASTReturnStatementNode* node) {
    string tabs;
    appendIndentsTo(tabs);
    cout<<tabs<<"<Return>"<<endl;
    increaseIndents();
    node->getReturnExp()->Accept(this);
    decreaseIndents();
    cout<<tabs<<"</Return>"<<endl;
}

void XMLVisitor::visit(ASTPrintStatementNode* node) {
    string tabs;
    appendIndentsTo(tabs);
    cout<<tabs<<"<Print>"<<endl;
    increaseIndents();
    node->getPrintStatement()->Accept(this);
    decreaseIndents();
    cout<<tabs<<"</Print>"<<endl;
}

void XMLVisitor::visit(ASTBlockStatementNode* node) {
    string tabs;
    appendIndentsTo(tabs);
    cout<<tabs<<"<Body>"<<endl;
    increaseIndents();
    for(auto statement: *node->getStatements()){
        if(statement != nullptr)
            statement->Accept(this);
    }
    decreaseIndents();
    cout<<tabs<<"</Body>"<<endl;
}

void XMLVisitor::visit(ASTAssignmentStatementNode* node){
    string tabs;
    appendIndentsTo(tabs);
    cout<<tabs<<"<Assignment>"<<endl;
    increaseIndents();
    tabs.append("\t");
    cout<<tabs<<"<ID>"<<node->getIdName()<<"</ID>"<<endl;
    node->getResult()->Accept(this);
    decreaseIndents();
    tabs = tabs.substr(0, tabs.length() - 1); // removing a tab to prevent re appending the entire string
    cout<<tabs<<"</Assignment>"<<endl;

}
void XMLVisitor::visit(ASTBinaryOpNode* node){
    string tabs;
    appendIndentsTo(tabs);
    string XMLIllegalChar;
    cout<<tabs<<"<BinaryOpNode Op=\""<<node->getOp()<<"\">"<<endl;
    increaseIndents();
    node->getLHS()->Accept(this);
    node->getRHS()->Accept(this);
    decreaseIndents();
    cout<<tabs<<"</BinaryOpNode>"<<endl;
}

void XMLVisitor::visit(ASTForLoopNode* node){
    string tabs;
    appendIndentsTo(tabs);
    cout<<tabs<<"<ForLoop>"<<endl;
    increaseIndents();
    tabs.append("\t");
    if(node->getDecleration() != nullptr) node->getDecleration()->Accept(this);
    cout<<tabs<<"<Cond>"<<endl;
    increaseIndents();
    tabs.append("\t");
    node->getCondition()->Accept(this);
    decreaseIndents();
    tabs = tabs.substr(0, tabs.length() - 1); // removing a tab to prevent re appending the entire string
    cout<<tabs<<"</Cond>"<<endl;

    if(node->getAssignment() != nullptr) node->getAssignment()->Accept(this);

    node->getBody()->Accept(this);

    decreaseIndents();
    tabs = tabs.substr(0, tabs.length() - 1); // removing a tab to prevent re appending the entire string
    cout<<tabs<<"</ForLoop>"<<endl;
}

void XMLVisitor::visit(ASTFunctionCallNode* node){
    string tabs;
    appendIndentsTo(tabs);
    cout<<tabs<<"<FunctionCall name=\""<<node->getFuncName()<<"\">"<<endl;
    increaseIndents();
    tabs.append("\t");
    for(auto param: *node->getParams()){
        cout<<tabs<<"<Argument>"<<endl;
        increaseIndents();
        param->Accept(this);
        decreaseIndents();
        cout<<tabs<<"</Argument>"<<endl;
    }
    decreaseIndents();
    tabs = tabs.substr(0, tabs.length() - 1); // removing a tab to prevent re appending the entire string
    cout<<tabs<<"</FunctionCall>"<<endl;
}

void XMLVisitor::visit(ASTFunctionDeclarationNode* node){
    string tabs;
    appendIndentsTo(tabs);
    cout<<tabs<<"<FuncDecl name=\""<<node->getFunctionName()<<"\">"<<endl;
    increaseIndents();
    tabs.append("\t");
    cout<<tabs<<"<FunctionType>"<<node->getType()<<"</FunctionType>"<<endl;
    if(node->getParams() != nullptr)
    {
        for(auto param : *node->getParams())
        {
            cout<<tabs<<"<Param>"<<endl;
            increaseIndents();
            param->Accept(this);
            decreaseIndents();
            cout<<tabs<<"</Param>"<<endl;
        }
    }
    node->getBody()->Accept(this);
    decreaseIndents();
    tabs = tabs.substr(0, tabs.length() - 1); // removing a tab to prevent re appending the entire string
    cout<<tabs<<"</FuncDecl>"<<endl;
}

void XMLVisitor::visit(ASTIfStatementNode* node){
    string tabs;
    appendIndentsTo(tabs);
    cout<<tabs<<"<IF>"<<endl;
    increaseIndents();
    tabs.append("\t");
    cout<<tabs<<"<COND>"<<endl;
    increaseIndents();
    node->getCondition()->Accept(this);
    cout<<tabs<<"</COND>"<<endl;
    decreaseIndents();
    node->getBlock()->Accept(this);

    if(node->getElseBlock() != nullptr){
        cout<<tabs<<"<ELSE>"<<endl;
        node->getElseBlock()->Accept(this);
        cout<<tabs<<"</ELSE>"<<endl;
    }
    decreaseIndents();
    tabs = tabs.substr(0, tabs.length() - 1); // removing a tab to prevent re appending the entire string
    cout<<tabs<<"</IF>"<<endl;
}
void XMLVisitor::visit(ASTLiteralNode* node){
    string tabs;
    appendIndentsTo(tabs);
    if(node->getType()=="TOK_INTEGERLIT") {
        cout << tabs << "<Integer>";
        cout<<node->getValue()<<"</Integer>"<<endl;
    }
    else if(node->getType()=="TOK_FLOATLIT"){
        cout << tabs << "<Float>";
        cout<<node->getValue()<<"</Float>"<<endl;
    }
    else if(node->getType()=="TOK_BOOLEANLIT"){
        cout << tabs << "<Bool>";
        bool value =(bool) node->getValue();
        cout<<value<<"</Bool>"<<endl;
    }
}

void XMLVisitor::visit(ASTParameterNode* node){
    string tabs;
    appendIndentsTo(tabs);
    cout<<tabs<<"<Param type=\""<<node->getType()<<"\">"<<node->getName()<<"</Param>"<<endl;
}

void XMLVisitor::visit(ASTUnaryNode* node){
    string tabs;
    appendIndentsTo(tabs);
    cout<<tabs<<"<Unary type=\""<<node->getType()<<"\">"<<endl;
    increaseIndents();
    node->getExp()->Accept(this);
    decreaseIndents();
    cout<<tabs<<"</Unary>"<<endl;
}

void XMLVisitor::visit(ASTVarDeclNode* node){
    string tabs;
    appendIndentsTo(tabs);
    cout<<tabs<<"<VarDecl>"<<endl;
    increaseIndents();
    tabs.append("\t");
    cout<<tabs<<"<Var type=\""<<node->getType()<<"\">"<<node->getVarName()<<"</Var>"<<endl;
    node->getValue()->Accept(this);
    decreaseIndents();
    tabs = tabs.substr(0, tabs.length() - 1); // removing a tab to prevent re appending the entire string
    cout<<tabs<<"</VarDecl>"<<endl;

}

void XMLVisitor::visit(ASTVariableIdentifierNode* node){
    string tabs;
    appendIndentsTo(tabs);
    cout<<tabs<<"<ID>"<<node->getName()<<"</ID>"<<endl;
}

void XMLVisitor::visit(ASTExpressionNode* node){
    //empty
}


void XMLVisitor::visit(ASTProgramNode* node){
    string tabs;
    appendIndentsTo(tabs);
    cout<<tabs<<"<Program>"<<endl;
    increaseIndents();
    for(auto statement : *node->getProgram()){
        if(statement!=nullptr)statement->Accept(this);
    }
    cout<<tabs<<"</Program>"<<endl;
}

void XMLVisitor::visit(ASTStatementNode* node){
    //empty
}

void XMLVisitor::increaseIndents() {indents++;}

void XMLVisitor::decreaseIndents() {indents--;}