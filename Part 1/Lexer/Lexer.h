#ifndef CPS2000_ASSIGNMENT_LEXER_H
#define CPS2000_ASSIGNMENT_LEXER_H

#include <iostream>
#include <stack>
#include <string>
#include <map>

#include "../Token/Token.h"

using namespace std;


typedef int State;


class Lexer {
public:
    Lexer();
    explicit Lexer(string* program);
    void loadProgram(string* program);
    Token getNextToken();

private:
    void initialiseReservedWords();
    TokenType checkReservedWord(const string& lexeme);

    string program;
    int lineNum;

    map<string, TokenType> reserved;
};


/*
 * Possible characters that can be read by the lexer.
 */
enum CharacterType {
    cSTART = 0,
    cUNKNOWN,
    cNEWLINE,       // \n
    cSPACE,         // isspace()

    cDIGIT,         // isdigit()

    cASTERISK,      // *
    cFSLASH,        // /
    cPLUS,          // +
    cMINUS,         // -
    cLESSTHAN,      // <
    cGREATERTHAN,   // >
    cEQUALS,        // =
    cEXCLAMATION,   // !

    cCOMMA,         // ,
    cPOINT,         // .
    cUNDERSCORE,    // _
    cQUOTATION,     // "
    cCOLON,         // :
    cSEMICOLON,     // ;
    cLBRACKET,      // (
    cRBRACKET,      // )
    cLCURLY,        // {
    cRCURLY,        // }

    cLETTER,        // isalpha()
    cPRINTABLE      // isprint()
};


#endif // CPS2000_ASSIGNMENT_LEXER_H
