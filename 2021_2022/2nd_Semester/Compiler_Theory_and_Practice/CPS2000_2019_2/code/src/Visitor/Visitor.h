#ifndef VISITOR_H
#define VISITOR_H

#include <sstream>
#include <stack>
#include <map>
#include <iostream>

#include "../Parser/AST.h"

using std::stringstream;
using std::stack;
using std::map;

class ASTNode;
class ASTNodeType;
class ASTNodeLiteral;
class ASTNodeIdentifier;
class ASTNodeMultiplicativeOp;
class ASTNodeAdditiveOp;
class ASTNodeRelationalOp;
class ASTNodeActualParams;
class ASTNodeFunctionCall;
class ASTNodeSubExpression;
class ASTNodeUnary;
class ASTNodeFactor;
class ASTNodeTerm;
class ASTNodeSimpleExpression;
class ASTNodeExpression;
class ASTNodeAssignment;
class ASTNodeVariableDecl;
class ASTNodePrintStatement;
class ASTNodeReturnStatement;
class ASTNodeIfStatement;
class ASTNodeForStatement;
class ASTNodeFormalParam;
class ASTNodeFormalParams;
class ASTNodeFunctionDecl;
class ASTNodeStatement;
class ASTNodeBlock;
class ASTNodeProgram;

class Visitor{
  public:
    Visitor(){};
    virtual ~Visitor(){};
    virtual void *visit(ASTNode*) = 0;
    virtual void *visit(ASTNodeType*) = 0;
    virtual void *visit(ASTNodeLiteral*) = 0;
    virtual void *visit(ASTNodeIdentifier*) = 0;
    virtual void *visit(ASTNodeMultiplicativeOp*) = 0;
    virtual void *visit(ASTNodeAdditiveOp*) = 0;
    virtual void *visit(ASTNodeRelationalOp*) = 0;
    virtual void *visit(ASTNodeActualParams*) = 0;
    virtual void *visit(ASTNodeFunctionCall*) = 0;
    virtual void *visit(ASTNodeSubExpression*) = 0;
    virtual void *visit(ASTNodeUnary*) = 0;
    virtual void *visit(ASTNodeFactor*) = 0;
    virtual void *visit(ASTNodeTerm*) = 0;
    virtual void *visit(ASTNodeSimpleExpression*) = 0;
    virtual void *visit(ASTNodeExpression*) = 0;
    virtual void *visit(ASTNodeAssignment*) = 0;
    virtual void *visit(ASTNodeVariableDecl*) = 0;
    virtual void *visit(ASTNodePrintStatement*) = 0;
    virtual void *visit(ASTNodeReturnStatement*) = 0;
    virtual void *visit(ASTNodeIfStatement*) = 0;
    virtual void *visit(ASTNodeForStatement*) = 0;
    virtual void *visit(ASTNodeFormalParam*) = 0;
    virtual void *visit(ASTNodeFormalParams*) = 0;
    virtual void *visit(ASTNodeFunctionDecl*) = 0;
    virtual void *visit(ASTNodeStatement*) = 0;
    virtual void *visit(ASTNodeBlock*) = 0;
    virtual void *visit(ASTNodeProgram*) = 0;
};

// clss for the XML visitor
class XMLVisitor : virtual public Visitor{
  public:
    XMLVisitor(){};
    virtual ~XMLVisitor(){};
    // All these return null since all is needed is to update the XML string
    virtual void *visit(ASTNode*){ return 0; };
    virtual void *visit(ASTNodeType *n);
    virtual void *visit(ASTNodeLiteral *n);
    virtual void *visit(ASTNodeIdentifier *n);
    virtual void *visit(ASTNodeMultiplicativeOp *n);
    virtual void *visit(ASTNodeAdditiveOp *n);
    virtual void *visit(ASTNodeRelationalOp *n);
    virtual void *visit(ASTNodeActualParams *n);
    virtual void *visit(ASTNodeFunctionCall *n);
    virtual void *visit(ASTNodeSubExpression *n);
    virtual void *visit(ASTNodeUnary *n);
    virtual void *visit(ASTNodeFactor *n);
    virtual void *visit(ASTNodeTerm *n);
    virtual void *visit(ASTNodeSimpleExpression *n);
    virtual void *visit(ASTNodeExpression *n);
    virtual void *visit(ASTNodeAssignment *n);
    virtual void *visit(ASTNodeVariableDecl *n);
    virtual void *visit(ASTNodePrintStatement *n);
    virtual void *visit(ASTNodeReturnStatement *n);
    virtual void *visit(ASTNodeIfStatement *n);
    virtual void *visit(ASTNodeForStatement *n);
    virtual void *visit(ASTNodeFormalParam *n);
    virtual void *visit(ASTNodeFormalParams *n);
    virtual void *visit(ASTNodeFunctionDecl *n);
    virtual void *visit(ASTNodeStatement *n);
    virtual void *visit(ASTNodeBlock *n);
    virtual void *visit(ASTNodeProgram *n);
    void *trimXMLNewLines(); // remove empty lines from xml
    string getXML(){ return xml.str(); }
  private:
    stringstream xml;
    unsigned int numberOfTabs = 0;
    string tabsString();
};

// Semantic Analysis
class SAVisitor : virtual public Visitor{
  public:
    SAVisitor(){};
    virtual ~SAVisitor(){};
    virtual void *visit(ASTNode*){ return 0; };
    virtual void *visit(ASTNodeType *n);
    virtual void *visit(ASTNodeLiteral *n);
    virtual void *visit(ASTNodeIdentifier *n);
    virtual void *visit(ASTNodeMultiplicativeOp *n);
    virtual void *visit(ASTNodeAdditiveOp *n);
    virtual void *visit(ASTNodeRelationalOp *n);
    virtual void *visit(ASTNodeActualParams *n);
    virtual void *visit(ASTNodeFunctionCall *n);
    virtual void *visit(ASTNodeSubExpression *n);
    virtual void *visit(ASTNodeUnary *n);
    virtual void *visit(ASTNodeFactor *n);
    virtual void *visit(ASTNodeTerm *n);
    virtual void *visit(ASTNodeSimpleExpression *n);
    virtual void *visit(ASTNodeExpression *n);
    virtual void *visit(ASTNodeAssignment *n);
    virtual void *visit(ASTNodeVariableDecl *n);
    virtual void *visit(ASTNodePrintStatement*);
    virtual void *visit(ASTNodeReturnStatement *n);
    virtual void *visit(ASTNodeIfStatement *n);
    virtual void *visit(ASTNodeForStatement *n);
    virtual void *visit(ASTNodeFormalParam *n);
    virtual void *visit(ASTNodeFormalParams *n);
    virtual void *visit(ASTNodeFunctionDecl *n);
    virtual void *visit(ASTNodeStatement *n);
    virtual void *visit(ASTNodeBlock *n);
    virtual void *visit(ASTNodeProgram *n);
    void printSymbolTable();
  private:
    vector<map<string, TokenType>> scope;
    void newScope(); // add scope as the 0 index of the vector
    void insert(string, TokenType); // in current scope
    void removeScope(); // remove scope at position 0
    // set as pointer to TokenType due to the need to make it return null
    TokenType* lookup(string); // lookup starting from vector 0 and going down
    // a map, mapping function names to their parameter types
    map <string, vector<TokenType> > functions; 
    TokenType *currentFunctionType = nullptr;
    int lineNumber = 0;

    // Methods to help if function has a proper return statement in all paths
    bool insideFor = false; // do nothing while inside for
    bool insideFunction = false; // only applies if inside function
    vector<bool> ifsReturn;
    int ifsReturnIndex = -1;
    bool goodReturn;
};

struct ValueType{
  void* value;
  TokenType type; //bool, int or float (needed for convertion)
  ValueType(){};
  ValueType(void* _value, TokenType _type){
    value = _value;
    type = _type;
  };
};

// Interpreter
class IVisitor : virtual public Visitor{
  public:
    IVisitor(){};
    virtual ~IVisitor(){};
    virtual void *visit(ASTNode*){ return 0; };
    virtual void *visit(ASTNodeType *n);
    virtual void *visit(ASTNodeLiteral *n);
    virtual void *visit(ASTNodeIdentifier *n);
    virtual void *visit(ASTNodeMultiplicativeOp *n);
    virtual void *visit(ASTNodeAdditiveOp *n);
    virtual void *visit(ASTNodeRelationalOp *n);
    virtual void *visit(ASTNodeActualParams *n);
    virtual void *visit(ASTNodeFunctionCall *n);
    virtual void *visit(ASTNodeSubExpression *n);
    virtual void *visit(ASTNodeUnary *n);
    virtual void *visit(ASTNodeFactor *n);
    virtual void *visit(ASTNodeTerm *n);
    virtual void *visit(ASTNodeSimpleExpression *n);
    virtual void *visit(ASTNodeExpression *n);
    virtual void *visit(ASTNodeAssignment *n);
    virtual void *visit(ASTNodeVariableDecl *n);
    virtual void *visit(ASTNodePrintStatement*);
    virtual void *visit(ASTNodeReturnStatement *n);
    virtual void *visit(ASTNodeIfStatement *n);
    virtual void *visit(ASTNodeForStatement *n);
    virtual void *visit(ASTNodeFormalParam *n);
    virtual void *visit(ASTNodeFormalParams *n);
    virtual void *visit(ASTNodeFunctionDecl *n);
    virtual void *visit(ASTNodeStatement *n);
    virtual void *visit(ASTNodeBlock *n);
    virtual void *visit(ASTNodeProgram *n);
    void printSymbolTable();
    void printValue(ValueType*);
    ValueType *getReturnValue(){ return returnValue; }
  private:
    vector<map<string, ValueType>> scope;
    void newScope(); // add scope as the 0 index of the vector
    void insert(string, ValueType); // in current scope
    void removeScope(); // remove scope at position 0
    ValueType* lookup(string); // lookup starting from vector 0 and going down
    // a map, mapping function names to wherever the function is
    map <string, ASTNode*> functions;
    int lineNumber = 0;

    bool performFunction = false;
    vector<ValueType> parameters;
    bool returnFromFunction = false;
    ValueType *returnValue = NULL;
};

#endif // VISITOR_H
