#include <string>
#include "Token.h"

using namespace std;


/*
 * Token Constructors
 */

Token::Token() {
    this->lineNum = 0;
    this->type = tREJECTED;
}

Token::Token(int lineNum, TokenType type) {
    this->lineNum = lineNum;
    this->type = type;
}

Token::Token(int lineNum, TokenType type, string* value) {
    this->lineNum = lineNum;
    this->type = type;
    this->value = *value;
}



/*
 * Returns a string representing the value of the token.
 * Line Number is also included.
 * If the token contains a special value (eg. IntegerLiteral, Identifier), this value will also be included.
 */
string Token::toString() {
    string s = "< ";
    s.append(to_string(lineNum));
    s.append(" : ");
    s.append(getTypeName());


    if (type == tFLOATLITERAL || type == tINTEGERLITERAL
        || type == tSTRINGLITERAL || type == tIDENTIFIER) {

        s.append(", ");
        s.append(value);

    }

    s.append(" >");

    return s;
}



/*
 * Returns a string value that can be used to identify the type of token stored.
 */
string Token::getTypeName() {
    switch(type)
    {
        case tREJECTED:
            return "Rejected";
        case tEND:
            return "End of File";
        case tSPACE:
            return "Space";
        case tBOOLEAN:
            return "bool";
        case tFLOAT:
            return "float";
        case tINTEGER:
            return "int";
        case tSTRING:
            return "string";
        case tFLOATLITERAL:
            return "FloatLiteral";
        case tINTEGERLITERAL:
            return "IntegerLiteral";
        case tSTRINGLITERAL:
            return "StringLiteral";
        case tTRUE:
            return "True";
        case tFALSE:
            return "False";
        case tIDENTIFIER:
            return "Identifier";
        case tAND:
            return "and";
        case tOR:
            return "or";
        case tNOT:
            return "not";
        case tASTERISK:
            return "*";
        case tFSLASH:
            return "/";
        case tPLUS:
            return "+";
        case tMINUS:
            return "-";
        case tLESSTHAN:
            return "<";
        case tGREATERTHAN:
            return ">";
        case tEQUALRELATIONAL:
            return "==";
        case tNOTEQUAL:
            return "!=";
        case tLESSTHANEQUAL:
            return "<=";
        case tGREATERTHANEQUAL:
            return ">=";
        case tEQUALASSIGN:
            return "=";
        case tCOMMA:
            return ",";
        case tCOLON:
            return ":";
        case tSEMICOLON:
            return ";";
        case tLBRACKET:
            return "(";
        case tRBRACKET:
            return ")";
        case tLCURLY:
            return "{";
        case tRCURLY:
            return "}";
        case tCOMMENT:
            return "Comment";
        case tELSE:
            return "else";
        case tFOR:
            return "for";
        case tIF:
            return "if";
        case tLET:
            return "let";
        case tPRINT:
            return "print";
        case tRETURN:
            return "return";
        case tWHILE:
            return "while";
        default:
            return "Unknown";
    }
}