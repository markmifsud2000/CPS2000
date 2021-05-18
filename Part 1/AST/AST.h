#ifndef AST_H
#define AST_H

#include <vector>
#include <string>

#include "../Visitor/Visitor.h"

using namespace std;


enum VariableType {
    INCOMPATIBLE = -1,  //Used when converting types during semantic analysis
    BOOL = 0,
    FLOAT,
    INT,
    STRING
};


enum Operator {
    MULT = 0,
    DIVIDE,
    PLUS,
    MINUS,

    LESSTHAN,
    LESSTHANEQUAL,
    GREATERTHAN,
    GREATERTHANEQUAL,
    EQUALS,
    NOTEQUALS,

    AND,
    OR,
    NOT,
};

string typeToString(VariableType t);


//Abstract Classes

class ASTNode {
public:
    virtual void accept(Visitor* v) = 0;
    int lineNum;    //Used when outputting error messages
};

class ASTStatement : public ASTNode {};
class ASTExpression : public ASTNode {};
class ASTLiteral : public ASTExpression {
    virtual VariableType getType() = 0; //Returns the type of the Literal stored
};




//Implemented Classes


//Root Node
class ASTProgram : public ASTNode {
public:
    ASTProgram(vector<ASTStatement*> program, int lineNum);
    void accept(Visitor* v) override;

    vector<ASTStatement*> program;
};



class ASTAssignment : public ASTStatement {
public:
    ASTAssignment(ASTIdentifier* identifier, ASTExpression* value, int lineNum);
    void accept(Visitor* v) override;

    ASTIdentifier* identifier;
    ASTExpression* value;
};


class ASTBinOp : public ASTExpression {
public:
    ASTBinOp(ASTExpression* lExpression, Operator op, ASTExpression* rExpression, int lineNum);
    void accept(Visitor* v) override;

    ASTExpression* lExpression;
    Operator op;
    ASTExpression* rExpression;
};


class ASTBlock : public ASTStatement {
public:
    explicit ASTBlock(vector<ASTStatement*> block, int lineNum);
    void accept(Visitor* v) override;

    vector<ASTStatement*> block; //Can be empty
};


class ASTFor : public ASTStatement {
public:
    ASTFor(ASTVariableDecl* declaration, ASTExpression* conditional, ASTAssignment* assignment,  ASTBlock* block, int lineNum);
    void accept(Visitor* v) override;

    ASTVariableDecl* declaration; //Can be null
    ASTExpression* conditional;
    ASTAssignment* assignment; //Can be null
    ASTBlock* block;
};


class ASTFormalParam : public ASTNode {
public:
    ASTFormalParam(ASTIdentifier* identifier, VariableType type, int lineNum);
    void accept(Visitor* v) override;

    ASTIdentifier* identifier;
    VariableType type;
};


class ASTFunctionCall : public ASTExpression {
public:
    ASTFunctionCall(ASTIdentifier* identifier, vector<ASTExpression*> param, int lineNum);
    void accept(Visitor* v) override;

    ASTIdentifier* identifier;
    vector<ASTExpression*> param;  //Can be empty
};


class ASTFunctionDecl : public ASTStatement {
public:
    ASTFunctionDecl(VariableType returnType, ASTIdentifier* identifier, vector<ASTFormalParam*> parameters, ASTBlock* block, int lineNum);
    void accept(Visitor* v) override;

    VariableType returnType;
    ASTIdentifier* identifier;
    vector<ASTFormalParam*> parameters; //Can be empty
    ASTBlock* block;
};


class ASTIdentifier : public ASTExpression {
public:
    ASTIdentifier(string identifier, int lineNum);
    void accept(Visitor* v) override;

    string identifier;
};


class ASTIf : public ASTStatement {
public:
    ASTIf(ASTExpression* conditional, ASTBlock* ifBlock, ASTBlock* elseBlock, int lineNum);
    void accept(Visitor* v) override;

    ASTExpression* conditional;
    ASTBlock* ifBlock;
    ASTBlock* elseBlock; //Can be null
};


class ASTLiteralBool : public ASTLiteral {
public:
    ASTLiteralBool(bool b, int lineNum);
    void accept(Visitor* v) override;
    VariableType getType() override;

    bool b;
};


class ASTLiteralFloat : public ASTLiteral {
public:
    ASTLiteralFloat(float f, int lineNum);
    void accept(Visitor* v) override;
    VariableType getType() override;

    float f;
};


class ASTLiteralInt : public ASTLiteral {
public:
    ASTLiteralInt(int i, int lineNum);
    void accept(Visitor* v) override;
    VariableType getType() override;

    int i;
};


class ASTLiteralString : public ASTLiteral {
public:
    ASTLiteralString(string s, int lineNum);
    void accept(Visitor* v) override;
    VariableType getType() override;

    string s;
};


class ASTPrint : public ASTStatement {
public:
    ASTPrint(ASTExpression* expression, int lineNum);
    void accept(Visitor* v) override;

    ASTExpression* expression;
};


class ASTReturn : public ASTStatement {
public:
    ASTReturn(ASTExpression* returnValue, int lineNum);
    void accept(Visitor* v) override;

    ASTExpression* returnValue;
};


class ASTUnary : public ASTExpression {
public:
    ASTUnary(Operator op, ASTExpression* expression, int lineNum);
    void accept(Visitor* v) override;

    Operator op;
    ASTExpression* expression;
};


class ASTVariableDecl : public ASTStatement{
public:
    ASTVariableDecl(ASTIdentifier* identifier, VariableType type, ASTExpression* value, int lineNum);
    void accept(Visitor* v) override;

    ASTIdentifier* identifier;
    VariableType type;
    ASTExpression* value;
};


class ASTWhile : public ASTStatement {
public:
    ASTWhile(ASTExpression* conditional, ASTBlock* block, int lineNum);
    void accept(Visitor* v) override;

    ASTExpression* conditional;
    ASTBlock* block;
};


#endif //AST_H
