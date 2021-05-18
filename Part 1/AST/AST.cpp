#include "AST.h"


/*
 * Convert a variable type to a string value.
 * Used for outputting error messages.
 */
string typeToString(VariableType t) {
    switch (t) {
        case BOOL:   return "bool";
        case FLOAT:  return "float";
        case INT:    return "int";
        case STRING: return "string";
        default:     return "unknown";
    }
}



/*
 * ASTProgram
 */

ASTProgram::ASTProgram(vector<ASTStatement*> program, int lineNum) {
    this->program = move(program);
    this->lineNum = lineNum;
}

void ASTProgram::accept(Visitor* v) {
    v->visit(this);
}



/*
 * ASTAssignment
 */

ASTAssignment::ASTAssignment(ASTIdentifier* identifier, ASTExpression* value, int lineNum) {
    this->identifier = identifier;
    this->value = value;
    this->lineNum = lineNum;
}


void ASTAssignment::accept(Visitor* v) {
    v->visit(this);
}


/*
 * ASTExpression
 */

ASTBinOp::ASTBinOp(ASTExpression *lExpression, Operator op, ASTExpression *rExpression, int lineNum) {
    this->lExpression = lExpression;
    this->op = op;
    this->rExpression = rExpression;
    this->lineNum = lineNum;
}

void ASTBinOp::accept(Visitor* v) {
    v->visit(this);
}


/*
 * ASTBlock
 */

ASTBlock::ASTBlock(vector<ASTStatement*> block, int lineNum) {
    this->block = move(block);
    this->lineNum = lineNum;
}

void ASTBlock::accept(Visitor* v) {
    v->visit(this);
}


/*
 * ASTFor
 */

ASTFor::ASTFor(ASTVariableDecl* declaration, ASTExpression* conditional, ASTAssignment* assignment, ASTBlock* block, int lineNum) {
    this->declaration = declaration;
    this->conditional = conditional;
    this->assignment = assignment;
    this->block = block;
    this->lineNum = lineNum;
}

void ASTFor::accept(Visitor* v) {
    v->visit(this);
}


/*
 * ASTFormalParam
 */

ASTFormalParam::ASTFormalParam(ASTIdentifier* identifier, VariableType type, int lineNum) {
    this->identifier = identifier;
    this->type = type;
    this->lineNum = lineNum;
}

void ASTFormalParam::accept(Visitor* v) {
    v->visit(this);
}


/*
 * ASTFunctionCall
 */

ASTFunctionCall::ASTFunctionCall(ASTIdentifier* identifier, vector<ASTExpression*> param, int lineNum) {
    this->identifier = identifier;
    this->param = move(param);
    this->lineNum = lineNum;
}

void ASTFunctionCall::accept(Visitor* v) {
    v->visit(this);
}


/*
 * ASTFunctionDecl
 */

ASTFunctionDecl::ASTFunctionDecl(VariableType returnType, ASTIdentifier* identifier, vector<ASTFormalParam*> parameters, ASTBlock* block, int lineNum) {
    this->returnType = returnType;
    this->identifier = identifier;
    this->parameters = move(parameters);
    this->block = block;
    this->lineNum = lineNum;
}

void ASTFunctionDecl::accept(Visitor* v) {
    v->visit(this);
}


/*
 * ASTIdentifier
 */


ASTIdentifier::ASTIdentifier(string identifier, int lineNum) {
    this->identifier = move(identifier);
    this->lineNum = lineNum;
}

void ASTIdentifier::accept(Visitor* v) {
    v->visit(this);
}


/*
 * ASTIf
 */

ASTIf::ASTIf(ASTExpression* conditional, ASTBlock* ifBlock, ASTBlock* elseBlock, int lineNum) {
    this->conditional = conditional;
    this->ifBlock = ifBlock;
    this->elseBlock = elseBlock;
    this->lineNum = lineNum;
}

void ASTIf::accept(Visitor* v) {
    v->visit(this);
}


/*
 * LiteralBool
 */

ASTLiteralBool::ASTLiteralBool(bool b, int lineNum) {
    this->b = b;
    this->lineNum = lineNum;
}

void ASTLiteralBool::accept(Visitor* v) {
    v->visit(this);
}

VariableType ASTLiteralBool::getType() {
    return BOOL;
}


/*
 * ASTLiteralFloat
 */

ASTLiteralFloat::ASTLiteralFloat(float f, int lineNum) {
    this->f = f;
    this->lineNum = lineNum;
}

void ASTLiteralFloat::accept(Visitor* v) {
    v->visit(this);
}

VariableType ASTLiteralFloat::getType() {
    return FLOAT;
}


/*
 * ASTLiteralInt
 */

ASTLiteralInt::ASTLiteralInt(int i, int lineNum) {
    this->i = i;
    this->lineNum = lineNum;
}

void ASTLiteralInt::accept(Visitor* v) {
    v->visit(this);
}

VariableType ASTLiteralInt::getType() {
    return INT;
}


/*
 * ASTLiteralString
 */

ASTLiteralString::ASTLiteralString(string s, int lineNum) {
    this->s = move(s);
    this->lineNum = lineNum;
}

void ASTLiteralString::accept(Visitor* v) {
    v->visit(this);
}

VariableType ASTLiteralString::getType() {
    return STRING;
}


/*
 * ASTPrint
 */

ASTPrint::ASTPrint(ASTExpression* expression, int lineNum) {
    this->expression = expression;
    this->lineNum = lineNum;
}

void ASTPrint::accept(Visitor* v) {
    v->visit(this);
}


/*
 * ASTReturn
 */

ASTReturn::ASTReturn(ASTExpression* returnValue, int lineNum) {
    this->returnValue = returnValue;
    this->lineNum = lineNum;
}

void ASTReturn::accept(Visitor* v) {
    v->visit(this);
}


/*
 * ASTUnary
 */

ASTUnary::ASTUnary(Operator op, ASTExpression* expression, int lineNum) {
    this->op = op;
    this->expression = expression;
    this->lineNum = lineNum;
}

void ASTUnary::accept(Visitor* v) {
    v->visit(this);
}


/*
 * ASTVariableDecl
 */

ASTVariableDecl::ASTVariableDecl(ASTIdentifier* identifier, VariableType type, ASTExpression* value, int lineNum) {
    this->identifier = identifier;
    this->type = type;
    this->value = value;
    this->lineNum = lineNum;
}

void ASTVariableDecl::accept(Visitor* v) {
    v->visit(this);
}


/*
 * ASTWhile
 */

ASTWhile::ASTWhile(ASTExpression* conditional, ASTBlock* block, int lineNum) {
    this->conditional = conditional;
    this->block = block;
    this->lineNum = lineNum;
}

void ASTWhile::accept(Visitor* v) {
    v->visit(this);
}

