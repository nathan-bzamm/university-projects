#ifndef AST_H
#define AST_H

#include <memory>
#include "TokenManager.h"
#include "../Visitor/Visitor.h"

using std::shared_ptr;

class Visitor;

class ASTNode{
    public:
      ASTNode(TokenManager *_tokenManager) {tokenManager = _tokenManager;};
      virtual ~ASTNode(){};
      virtual bool parse() = 0; //will return true if parse was successful
      virtual void *accept(Visitor *v) = 0; 
    protected:
        TokenManager *tokenManager;
        //match a TokenType and make sure it exists
        Token *match(TokenType tokenType);
};

class ASTNodeType : virtual public ASTNode{
    public:
      //the constructor is the same as the parent
      ASTNodeType(TokenManager *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeType(){};
      virtual bool parse(); //will return true if parse was successful
      virtual void *accept(Visitor *visitor);

      Token* token;
};

class ASTNodeLiteral : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeLiteral(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeLiteral(){};
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      Token* token;
};


class ASTNodeIdentifier : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeIdentifier(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeIdentifier(){};
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      Token* token;
};

class ASTNodeMultiplicativeOp : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeMultiplicativeOp(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeMultiplicativeOp(){};
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      Token* token;
};

class ASTNodeAdditiveOp : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeAdditiveOp(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeAdditiveOp(){};
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      Token* token;
};

class ASTNodeRelationalOp : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeRelationalOp(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeRelationalOp(){};
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      Token* token;
};

class ASTNodeActualParams : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeActualParams(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeActualParams(){};
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      vector<ASTNode*> expressions;
};

class ASTNodeFunctionCall : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeFunctionCall(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeFunctionCall();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      ASTNode* identifier = NULL;
      ASTNode* actualParams = NULL;
};

class ASTNodeSubExpression : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeSubExpression(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeSubExpression();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      ASTNode* expression = NULL;
};

class ASTNodeUnary : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeUnary(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeUnary();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      Token* token;
      ASTNode* expression = NULL;
};

class ASTNodeFactor : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeFactor(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeFactor();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      ASTNode* node = NULL;
};

class ASTNodeTerm : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeTerm(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeTerm();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      vector<ASTNode*> factors;
      vector<ASTNode*> multiplicativeOP;
};

class ASTNodeSimpleExpression : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeSimpleExpression(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeSimpleExpression();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      vector<ASTNode*> additiveOP;
      vector<ASTNode*> terms;
};

class ASTNodeExpression : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeExpression(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeExpression();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      vector<ASTNode*> relationalOp;
      vector<ASTNode*> simpleExpressions;
};

class ASTNodeAssignment : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeAssignment(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeAssignment();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      ASTNode *identifier;
      ASTNode *expression;
};

class ASTNodeVariableDecl : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeVariableDecl(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeVariableDecl();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      ASTNode* identifier = NULL;
      ASTNode* type = NULL;
      ASTNode* expression = NULL;
};

class ASTNodePrintStatement : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodePrintStatement(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodePrintStatement();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      ASTNode* expression = NULL;
};

class ASTNodeReturnStatement : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeReturnStatement(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeReturnStatement();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      ASTNode* expression = NULL;
};

class ASTNodeIfStatement : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeIfStatement(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeIfStatement();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      ASTNode* expression = NULL;
      ASTNode* block = NULL;
      ASTNode* elseBlock = NULL;
};

class ASTNodeForStatement : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeForStatement(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeForStatement();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      ASTNode* variableDecl = NULL;
      ASTNode* expression = NULL;
      ASTNode* assignment = NULL;
      ASTNode* block = NULL;
};

class ASTNodeFormalParam : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeFormalParam(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeFormalParam();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      ASTNode* identifier = NULL;
      ASTNode* type = NULL;
};

class ASTNodeFormalParams : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeFormalParams(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeFormalParams();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      vector<ASTNode*> formalParams;
};

class ASTNodeFunctionDecl : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeFunctionDecl(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeFunctionDecl();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      ASTNode* identifier = NULL;
      ASTNode* formalParams = NULL;
      ASTNode* type = NULL;
      ASTNode* block = NULL;
};

class ASTNodeStatement : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeStatement(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeStatement();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      ASTNode* statement = NULL;
};

class ASTNodeBlock : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeBlock(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeBlock();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      vector<ASTNode*> statements;
};

class ASTNodeProgram : virtual public ASTNode{
    public:
      // constructor is the same as the parent
      ASTNodeProgram(TokenManager  *tokenManager) : ASTNode(tokenManager) {};
      virtual ~ASTNodeProgram();
      virtual bool parse(); // will return true if parse was successful
      virtual void *accept(Visitor *v);

      vector<ASTNode*> statements;
};

#endif // ASTNODE_H