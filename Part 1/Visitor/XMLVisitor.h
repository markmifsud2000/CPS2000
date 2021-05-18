#ifndef CPS2000_ASSIGNMENT_XMLVISITOR_H
#define CPS2000_ASSIGNMENT_XMLVISITOR_H

#include <string>
#include <fstream>
#include "Visitor.h"

#define DEFAULT_FILENAME "AST.xml"

using namespace std;


class XMLVisitor : public Visitor {
public:
    XMLVisitor();
    explicit XMLVisitor(const string& filename);
    ~XMLVisitor();

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
    int indent;
    ofstream file;

    void addIndent();
};


#endif //CPS2000_ASSIGNMENT_XMLVISITOR_H
