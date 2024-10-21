//
// Created by aidan on 17/05/2019.
//

#include "Visitor.h"
#include <vector>
#include <string>
#include <map>

class InterpreterVisitor: public Visitor{
private:
    enum idType{VAR, FUNC};
    enum dataType{INT,FLT,BOOL};

    struct tableEntry{
        dataType dType;
        idType entryType;
        float value;
        ASTStatementNode* funcBody;
        std::vector<ASTParameterNode*>* parameters; //holds the name of the parameters to be checked in the symbol table

        tableEntry(dataType d, idType t, float val, ASTStatementNode* body, std::vector<ASTParameterNode*>* pars){
            dType = d;
            entryType = t;
            parameters = pars;
            value = val;
            funcBody = body;
        }
    };//tableEntry;

    std::vector<std::map<std::string, tableEntry*>*>* symbolTable;

    std::vector<float>* resultStack; //hold the top most result for interpreting

    void STPush(); // pushes a new map on the stack (new scope)
    void STInsert(const std::string &name,dataType dType, idType entryType,float val = 0,
            ASTStatementNode* body = nullptr,std::vector<ASTParameterNode*>* params= nullptr); //enters a variable in the top most map
    tableEntry* STLookup(std::string name);
    void STUpdate(const std::string &id, float newVal);
    void STPop(); //pops out the current map/scope

    void resultStackPush(float r);
    float resultStackPop();


    dataType dataTypeToEnum(const std::string &type);


public:
    InterpreterVisitor();
    ~InterpreterVisitor();
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
