#ifndef CPS2000_ASSIGNMENT_PARSER_H
#define CPS2000_ASSIGNMENT_PARSER_H

#include <iostream>
#include <stack>
#include <string>

#include "../Lexer/Lexer.h"
#include "../Token/Token.h"

#include "../AST/AST.h"

using namespace std;


class Parser {
public:
    Parser();
    explicit Parser(string* program);
    void loadProgram(string* program);

    ASTProgram* parseProgram();

private:
    Lexer lexer;
    Token next;     //Lookahead token
    Token nextnext; //Used when two lookahead tokens are required

    Token getNextToken();

    ASTStatement* parseStatement();
    ASTExpression* parseFactor();
    ASTLiteral* parseLiteral();

    ASTAssignment* parseAssignment();
    ASTBlock* parseBlock();
    ASTExpression* parseExpression();
    ASTFor* parseFor();
    ASTFormalParam* parseFormalParam();
    ASTFunctionCall* parseFunctionCall();
    ASTFunctionDecl* parseFunctionDecl();
    ASTIdentifier* parseIdentifier();
    ASTIf* parseIf();
    //parseLiteralBool, parseLiteralInt, etc.
    ASTPrint* parsePrint();
    ASTReturn* parseReturn();
    ASTExpression* parseSimpleExpression();
    ASTExpression* parseSubExpression();
    ASTExpression* parseTerm();
    ASTUnary* parseUnary();
    ASTVariableDecl* parseVariableDecl();
    ASTWhile* parseWhile();
};


#endif // CPS2000_ASSIGNMENT_PARSER_H