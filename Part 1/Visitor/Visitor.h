#ifndef CPS2000_ASSIGNMENT_VISITOR_H
#define CPS2000_ASSIGNMENT_VISITOR_H

//Temporary class declarations
class ASTProgram;
class ASTAssignment;
class ASTBinOp;
class ASTBlock;
class ASTFor;
class ASTFormalParam;
class ASTFunctionCall;
class ASTFunctionDecl;
class ASTIdentifier;
class ASTIf;
class ASTLiteralBool;
class ASTLiteralFloat;
class ASTLiteralInt;
class ASTLiteralString;
class ASTPrint;
class ASTReturn;
class ASTSubExpression;
class ASTUnary;
class ASTVariableDecl;
class ASTWhile;


/*
 * Visitor design pattern.
 * Used to access a file
 */
class Visitor {
public:
    virtual void visit(ASTProgram*) = 0;
    virtual void visit(ASTAssignment*) = 0;
    virtual void visit(ASTBinOp*) = 0;
    virtual void visit(ASTBlock*) = 0;
    virtual void visit(ASTFor*) = 0;
    virtual void visit(ASTFormalParam*) = 0;
    virtual void visit(ASTFunctionCall*) = 0;
    virtual void visit(ASTFunctionDecl*) = 0;
    virtual void visit(ASTIdentifier*) = 0;
    virtual void visit(ASTIf*) = 0;
    virtual void visit(ASTLiteralBool*) = 0;
    virtual void visit(ASTLiteralFloat*) = 0;
    virtual void visit(ASTLiteralInt*) = 0;
    virtual void visit(ASTLiteralString*) = 0;
    virtual void visit(ASTPrint*) = 0;
    virtual void visit(ASTReturn*) = 0;
    virtual void visit(ASTUnary*) = 0;
    virtual void visit(ASTVariableDecl*) = 0;
    virtual void visit(ASTWhile*) = 0;
};

#endif //CPS2000_ASSIGNMENT_VISITOR_H
