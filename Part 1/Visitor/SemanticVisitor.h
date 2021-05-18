#ifndef CPS2000_ASSIGNMENT_SEMANTICVISITOR_H
#define CPS2000_ASSIGNMENT_SEMANTICVISITOR_H

#include <string>
#include "Visitor.h"
#include "../SymbolTable/SymbolTable.h"

using namespace std;


class SemanticVisitor : public Visitor {
public:
    SemanticVisitor();

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
    SymbolTable table;          //The stack of symbol tables, each table in the stack corresponds to a scope
    VariableType returnedType;  //The variable type returned by the last node that was visited.
    bool returnStatement;       //True if the last statement was a return statement, used in function declaration

};



#endif //CPS2000_ASSIGNMENT_SEMANTICVISITOR_H
