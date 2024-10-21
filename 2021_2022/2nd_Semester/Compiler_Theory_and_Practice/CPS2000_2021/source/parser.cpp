//
// Created by aidan on 30/04/2019.
//

#include "parser.h"
#include "XMLVisitor.h"
#include "InterpreterVisitor.h"
#include "SemanticAnalysisVisitor.h"
#include <iostream>

using namespace std;

Parser::Parser(Lexer* lex){
    lexer = lex;
};

ASTVarDeclNode* Parser::parseVarDeclaration() {
    CurrentToken = lexer->getNextToken();
    string name;
    string type;
    ASTExpressionNode* expression;

    if(CurrentToken->tokenType == TOK_ID){
        name = CurrentToken->name;
        CurrentToken = lexer->getNextToken();
    }
    else{
        Error("Error: Expecting an Identifier in variable declaration at line ",CurrentToken->lineNumber);
        return nullptr;
    }

    if(CurrentToken->tokenType == TOK_COLON) CurrentToken = lexer->getNextToken(); // discard semicolon
    else{
        Error("Error: Expecting a \':\' in variable declaration at line ",CurrentToken->lineNumber);
        return nullptr;
    }

    if(CurrentToken->tokenType == TOK_TYPE){
        type = CurrentToken->name;
        CurrentToken = lexer->getNextToken();
    }
    else{
        Error("Error: Invalid type in variable declaration at line ",CurrentToken->lineNumber);
        return nullptr;
    }

    if(CurrentToken->tokenType != TOK_EQUALSSIGN){
        Error("Error: Expecting an \'=\' in variable declaration at line ",CurrentToken->lineNumber);
        return nullptr;
    }

    expression = parseExpression();
    return new ASTVarDeclNode(name, type, expression);
}

ASTExpressionNode* Parser::parseExpression(){
    auto lhs = parseSimpleExpression();

    //checking if there is a relop in the middle of two expressions
    if(isRelop(CurrentToken->tokenType)){
      string op = CurrentToken->name;
        auto rhs = parseSimpleExpression();
        return new ASTBinaryOpNode(lhs, op, rhs);
    }
    else return lhs;
}

ASTExpressionNode* Parser::parseSimpleExpression() {
    auto lhs = parseTerm();

    //checking if there is a relop in the middle of two expressions
    if(isMultOp(CurrentToken->tokenType)){
        string op = CurrentToken->name;
        auto rhs = parseTerm();
        return new ASTBinaryOpNode(lhs, op, rhs);
    }
    else return lhs;
}

ASTExpressionNode* Parser::parseTerm() {
    CurrentToken = lexer->getNextToken();

    ASTExpressionNode* lhs = nullptr; // just assigning this to avoid warnings

    //Checking if it's just a single term
    if(CurrentToken->tokenType == TOK_ID){ //handles identifiers and function calls
        lhs = parseIdentifierExpression(); //parseId
    }
    else if(CurrentToken->tokenType == TOK_INTEGERLIT || CurrentToken->tokenType == TOK_BOOLEANLIT || CurrentToken->tokenType == TOK_FLOATLIT){
        lhs = new ASTLiteralNode(tokenToString.at(CurrentToken->tokenType),CurrentToken->value);
        CurrentToken = lexer->getNextToken();
    }
    else if(CurrentToken->tokenType == TOK_LBRACK){ //sub expression
        auto node = parseExpression();
        if(CurrentToken->tokenType == TOK_RBRACK){
            lhs = node;
        }
        else{
            Error("Expecting a ) after sub expression in line ",CurrentToken->lineNumber);
            lhs = nullptr;
        }
    }
    else if(CurrentToken->tokenType == TOK_MINUS || CurrentToken->tokenType == TOK_NOT){ //Unary expression
        string name = CurrentToken->name;
        auto node = parseExpression();
        lhs = new ASTUnaryNode(name,node);
    }

    //Checking whether to parse another expression or not
    if(isAddOp(CurrentToken->tokenType)){
        string op = CurrentToken->name;
        auto rhs = parseSimpleExpression();
        return new ASTBinaryOpNode(lhs, op, rhs);
    }

    if(lhs == nullptr){
        Error("Expecting an expression in line ",CurrentToken->lineNumber);
    }
    return lhs;
}

ASTExpressionNode* Parser::parseIdentifierExpression() {
    string name = CurrentToken->name;

    CurrentToken = lexer->getNextToken();
    if(CurrentToken->tokenType == TOK_LBRACK){ // function call
        auto params = new vector<ASTExpressionNode*>;
        if(lexer->peekNextToken()->tokenType!=TOK_RBRACK) {
            do { // getting the parameters until no more commas encountered
                ASTExpressionNode* node = parseExpression();
                if (node != nullptr) params->push_back(node);
                else break;
            } while (CurrentToken->tokenType == TOK_COMMA);
        }
        if(CurrentToken->tokenType != TOK_RBRACK){
            Error("Expecting a \')\' after function call in line ",CurrentToken->lineNumber);
            return nullptr;
        }
        else{
            CurrentToken = lexer->getNextToken();
            return new ASTFunctionCallNode(name, params);
        }
    }
    else{ // else it's a variable
        return new ASTVariableIdentifierNode(name); //ID Node
    }
}

bool Parser::isAddOp(TOK t){
    return t==TOK_ADD || t ==TOK_MINUS || t==TOK_OR;
}

bool Parser::isMultOp(TOK t) {
    return  t==TOK_MULT || t==TOK_DIV|| t==TOK_AND;
}

bool Parser::isRelop(TOK t){
    return t==TOK_LT || t==TOK_GT || t==TOK_EQ || t==TOK_NE || t==TOK_GE || t==TOK_LE;
}

ASTStatementNode* Parser::parseIfStatement() {
    CurrentToken = lexer->getNextToken();

    ASTExpressionNode* condition= nullptr;
    ASTStatementNode* block = nullptr;
    ASTStatementNode* elseBlock = nullptr;

    if(CurrentToken->tokenType == TOK_LBRACK){
        condition = parseExpression();
    }
    else{
        Error("Expecting an expression in the If statement in line ",CurrentToken->lineNumber);
        return nullptr;
    }

    if(CurrentToken->tokenType != TOK_RBRACK){
        Error("Expecting a \')\'  in the If statement in line ",CurrentToken->lineNumber);
        return nullptr;
    }

    CurrentToken = lexer->getNextToken();
    block = parseBlock();

    Token* lookahead = lexer->peekNextToken();

    if(lookahead->tokenType==TOK_ELSE) {
        CurrentToken = lexer->getNextToken();
        CurrentToken = lexer->getNextToken();
        elseBlock = parseBlock();
    }

    return new ASTIfStatementNode(condition, block, elseBlock);
}

ASTStatementNode* Parser::parseBlock(){
    if(CurrentToken->tokenType != TOK_BEGIN_SCOPE){
        Error("Expecting a \'{\' at beginning of scope at line ",CurrentToken->lineNumber);
    }
    else{
        auto statements = new vector<ASTStatementNode*>;
        ASTStatementNode* nodeToPush;
        do{
            nodeToPush = parseStatement();
            statements->push_back(nodeToPush);
        }while(nodeToPush != nullptr);
        if(CurrentToken->tokenType == TOK_END_SCOPE) return new ASTBlockStatementNode(statements);
        else{
            Error("Expecting a \'}\' at the end of scope in line ", CurrentToken->lineNumber);
        }

    }
}

ASTStatementNode* Parser::parseAssignmentStatement(){
    string name = CurrentToken->name;

    ASTExpressionNode* node = nullptr;
    CurrentToken = lexer->getNextToken();

    if(CurrentToken->tokenType == TOK_EQUALSSIGN){
        node = parseExpression();
        return new ASTAssignmentStatementNode(name,node);
    }
    else{
        Error("Expecting an \'=\' in assignment statement in line ", CurrentToken->lineNumber);
    }
}

ASTStatementNode* Parser::parseFunctionDeclaration() {
    CurrentToken = lexer->getNextToken();

    string name;
    string funcType;
    auto params = new vector<ASTParameterNode*>;
    ASTStatementNode* body;

    if(CurrentToken->tokenType == TOK_ID){
        name = CurrentToken->name;
        CurrentToken = lexer->getNextToken();
    }
    else
        Error("Expecting an identifier in function declaration in line ", CurrentToken->lineNumber);

    if(CurrentToken->tokenType==TOK_LBRACK) CurrentToken = lexer->getNextToken(); // discarding bracket
    else Error("Expecting a \'(\' in function declaration in line ", CurrentToken->lineNumber);

    if(CurrentToken->tokenType != TOK_RBRACK) {
        //Parse parameters
        while (true) {
            string parname;
            string partype;
            if (CurrentToken->tokenType == TOK_ID) {
                parname = CurrentToken->name;
                CurrentToken = lexer->getNextToken();
            } else Error("Expecting an identifier name in line ", CurrentToken->lineNumber);

            if (CurrentToken->tokenType == TOK_COLON)CurrentToken = lexer->getNextToken();
            else Error("Expecting a \':\' in parameter definition in line ", CurrentToken->lineNumber);

            if (CurrentToken->tokenType == TOK_TYPE) {
                partype = CurrentToken->name;
                CurrentToken = lexer->getNextToken();
            } else Error("Expecting a \'valid type\' in parameter definition in line ", CurrentToken->lineNumber);

            if (CurrentToken->tokenType == TOK_COMMA) {
                params->push_back(new ASTParameterNode(parname, partype));
                CurrentToken = lexer->getNextToken();
            } else if (CurrentToken->tokenType == TOK_RBRACK) {
                params->push_back(new ASTParameterNode(parname, partype));
                CurrentToken = lexer->getNextToken();
                break;
            } else Error("Expecting a \')\' after parameter definition in line ", CurrentToken->lineNumber);
        }
    }
    else
        CurrentToken = lexer->getNextToken();
    if(CurrentToken->tokenType == TOK_COLON) CurrentToken = lexer->getNextToken();
    else Error("Expecting a \':\' before function type in line ", CurrentToken->lineNumber);

    if(CurrentToken->tokenType == TOK_TYPE){
        funcType = CurrentToken->name;
        CurrentToken = lexer->getNextToken();
    }
    else Error("Expecting a \'valid type\' in line ", CurrentToken->lineNumber);

    body = parseBlock();
    return new ASTFunctionDeclarationNode(name, params, funcType, body);
}

ASTStatementNode* Parser::parseForLoopStatement() {
    ASTStatementNode* declaration = nullptr;
    ASTExpressionNode* condition = nullptr;
    ASTStatementNode* assignment = nullptr;
    ASTStatementNode* body= nullptr;

    CurrentToken = lexer->getNextToken();

    if(CurrentToken->tokenType == TOK_LBRACK) CurrentToken = lexer->getNextToken(); // discard '('
    else Error("Expecting a \'(\' in for loop statement in line ", CurrentToken->lineNumber);

    if(CurrentToken->tokenType == TOK_VARDECL)
        declaration = parseVarDeclaration();


    if(CurrentToken->tokenType != TOK_SEMICOLON) Error("Expecting \';\' after declaration in for loop at line ", CurrentToken->lineNumber);

    condition = parseExpression();

    if(CurrentToken->tokenType == TOK_SEMICOLON) CurrentToken = lexer->getNextToken();
    else Error("Expecting \';\' after expression in for loop at line ", CurrentToken->lineNumber);

    if(CurrentToken->tokenType == TOK_ID) assignment = parseAssignmentStatement();

    if(CurrentToken->tokenType == TOK_RBRACK){
        CurrentToken = lexer->getNextToken();
        body = parseBlock();
    }

    return new ASTForLoopNode(declaration, condition, assignment, body);

}

ASTStatementNode* Parser::parseStatement() {
    ASTStatementNode* node = nullptr;
    CurrentToken = lexer->getNextToken();
    switch(CurrentToken->tokenType){ //deciding what type of statement
        case TOK_RETURN:{
            auto node = parseExpression();
            if(CurrentToken->tokenType != TOK_SEMICOLON)
                Error("Expecting \';\' after 'return' in line ", CurrentToken->lineNumber);
            return new ASTReturnStatementNode(node);
        }
        case TOK_VARDECL:{
            node = parseVarDeclaration();
            if(CurrentToken->tokenType != TOK_SEMICOLON)
                Error("Expecting \';\' after 'var declaration' in line ", CurrentToken->lineNumber);
            break;
        }
        case TOK_PRINT: {
            auto node = parseExpression();
            if(CurrentToken->tokenType != TOK_SEMICOLON)
                Error("Expecting \';\' after 'print' statement in line ", CurrentToken->lineNumber);
            return new ASTPrintStatementNode(node);
        }
        case TOK_IF: node = parseIfStatement();break;
        case TOK_FOR: node = parseForLoopStatement();break;
        case TOK_BEGIN_SCOPE: node = parseBlock();break;
        case TOK_ID:{
            node = parseAssignmentStatement();
            if(CurrentToken->tokenType != TOK_SEMICOLON)
                Error("Expecting \';\' after 'print' statement in line ", CurrentToken->lineNumber);
            break;
        }
        case TOK_FUNCDECL: node = parseFunctionDeclaration();break;
        default:{
            if(CurrentToken->tokenType != TOK_END_SCOPE && CurrentToken->tokenType != TOK_EOF){
                Error("Invalid statement in line ", CurrentToken->lineNumber);
            }
        };
    }
    return node;
}

//! Displays errors
void Parser::Error(const std::string &errMsg, int lineNumber) {
    cout<<"Compilation error!"<<endl;
    cerr<<errMsg<<lineNumber<<endl;
    exit(EXIT_FAILURE);
}

ASTNode* Parser::parse() {
    auto programStatements =  new vector<ASTStatementNode*>;
    auto XMLv = new XMLVisitor;
    auto interpter = new InterpreterVisitor();
    auto SemanticVisitor = new SemanticAnalysisVisitor;
    programStatements->push_back(parseStatement());
    while(CurrentToken->tokenType != TOK_EOF){
        auto nodeToPush = parseStatement();
        programStatements->push_back(nodeToPush);
    }
    Root = new ASTProgramNode(programStatements);
    cout<<"Finished parsing. Outputting XML"<<endl;
    Root->Accept(XMLv);
    cout<<"Finished outputting XML. Analyzing semantics"<<endl;
    Root->Accept(SemanticVisitor);
    cout<<"Finished analyzing semantics"<<endl;
    cout<<"Executing program"<<endl;
    Root->Accept(interpter);
    cout<<"Finished execution"<<endl;
}