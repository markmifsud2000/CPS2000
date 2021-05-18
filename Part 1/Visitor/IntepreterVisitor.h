#ifndef CPS2000_ASSIGNMENT_INTEPRETERVISITOR_H
#define CPS2000_ASSIGNMENT_INTEPRETERVISITOR_H


#include <string>
#include "Visitor.h"
#include "../SymbolTable/SymbolTable.h"

using namespace std;


class InterpreterVisitor : public Visitor {
public:
    InterpreterVisitor();

    void visit(ASTProgram*) override;
    void visit(ASTAssignment*) override;
    void visit(ASTBinOp*) override;
    void visit(ASTBlock*) override;
    void visit(ASTFor*) override;
    void visit(ASTFormalParam*) override;
    void visit(ASTFunctionCall*) override;
    void visit(ASTFunctionDecl*) override;
    void visit(ASTIdentifier*) override;
    void visit(ASTIf*) override;
    void visit(ASTLiteralBool*) override;
    void visit(ASTLiteralFloat*) override;
    void visit(ASTLiteralInt*) override;
    void visit(ASTLiteralString*) override;
    void visit(ASTPrint*) override;
    void visit(ASTReturn*) override;
    void visit(ASTUnary*) override;
    void visit(ASTVariableDecl*) override;
    void visit(ASTWhile*) override;


private:
    void convertReturnedType(VariableType type);

    SymbolTable table;          //The stack of symbol tables, each table in the stack corresponds to a scope

    //Store return values temporarily
    bool returnedBool;
    float returnedFloat;
    int returnedInt;
    string returnedString;

    //Track the type of the last returned value
    //Used to determine which return value to use
    VariableType returnedType;

};



#endif //CPS2000_ASSIGNMENT_INTEPRETERVISITOR_H
