#include "Lexer.h"
#include "LexerTable.h"

using namespace std;


CharacterType charCat(char c);
int countLines(string s);
template <class T>
stack<T> clearStack(stack<T> s);




Lexer::Lexer() {
    initialiseReservedWords();
    this->lineNum = 1;
}


Lexer::Lexer(string* program) {
    initialiseReservedWords();
    this->program = *program;
    this->lineNum = 1;
}


/*
 * Sets the program string to be compiled.
 */
void Lexer::loadProgram(string* program) {
    this->program = *program;
    this->lineNum = 1;
}


/*
 * Returns the next token in the program.
 */
Token Lexer::getNextToken() {

    Token t;
    bool tokenFound;

    // Loop until a (non-whitespace/non-comment) token is found
    do {

        //Check if we have reached the end of the program
        if (program.empty()) {
            //We have reached the end of the program
            t.lineNum = lineNum;
            t.type = tEND;
            return t;
        }



        //Initialisation
        State s = cSTART;
        string lexeme;
        stack <State> stack;
        stack.push(-1);     //Push "bad" state
        int i = 0;



        //Scanning Loop
        while ((s != E) && (i < program.size())) {

            //Read the next character
            char c = program[i];
            lexeme += c;

            if (accepting[s] != tREJECTED) {
                //This is an accepting state
                stack = clearStack(stack);
            }

            stack.push(s);

            //Check what type of character has been read
            CharacterType cType = charCat(c);

            //Perform the DFA state change using the lookup table
            s = table[s][cType];

            i++;
        }


        string errorLexeme = lexeme; //Make a copy of the lexeme which can be used when reporting errors.


        //Rollback Loop
        while ((s != -1) && (accepting[s] == tREJECTED)) {
            s = stack.top();
            stack.pop();

            lexeme = lexeme.substr(0, lexeme.size() - 1);
        }



        //Report Result


        //Check if Token is accepted and check for special cases

        if (accepting[s] == tSPACE || accepting[s] == tCOMMENT) {
            //Ignore Whitespaces and Comments.
            tokenFound = false;
        }

        else if (accepting[s] == tIDENTIFIER) {
            //Accepted Token
            //This is an identifier, check if it is a reserved word

            t.lineNum = lineNum;

            TokenType type = checkReservedWord(lexeme);

            t.type = type;
            if (type == tIDENTIFIER) {
                //If it is an identifier, include the identifier name
                t.value = lexeme;
            }

            tokenFound = true;
        }

        else if (accepting[s] == tFLOATLITERAL || accepting[s] == tINTEGERLITERAL || accepting[s] == tSTRINGLITERAL) {

            //Accepted Token
            //Include the value with the token

            t.lineNum = lineNum;
            t.type = accepting[s];
            t.value = lexeme;
            tokenFound = true;
        }

        else if ((accepting[s] != tREJECTED) && (s != -1)) {
            //Accepted Token
            t.lineNum = lineNum;
            t.type = accepting[s];
            tokenFound = true;
        }

        else {
            //Rejected Token

            //Report error
            string error = "Lexical Error on line " + to_string(lineNum) + "!\nLexeme: " + errorLexeme + "\n";
            throw runtime_error(error);
        }


        //Update the program number
        lineNum += countLines(lexeme);

        //Remove the lexeme from the program
        program = program.substr(lexeme.length());


    } while (!tokenFound);


    return t;
}



/*
 * Determines the category of a character read by the lexer.
 * Each character category represents a column in the lookup table.
 */
CharacterType charCat(char c) {

    if (c == '\n')
        return cNEWLINE;
    else if (isspace(c))
        return cSPACE;

    else if (isdigit(c))
        return cDIGIT;

    else if (c == '*')
        return cASTERISK;
    else if (c == '/')
        return cFSLASH;
    else if (c == '+')
        return cPLUS;
    else if (c == '-')
        return cMINUS;
    else if (c == '<')
        return cLESSTHAN;
    else if (c == '>')
        return cGREATERTHAN;
    else if (c == '=')
        return cEQUALS;
    else if (c == '!')
        return cEXCLAMATION;

    else if (c == ',')
        return cCOMMA;
    else if (c == '.')
        return cPOINT;
    else if (c == '_')
        return cUNDERSCORE;
    else if (c == '"')
        return cQUOTATION;
    else if (c == ':')
        return cCOLON;
    else if (c == ';')
        return cSEMICOLON;
    else if (c == '(')
        return cLBRACKET;
    else if (c == ')')
        return cRBRACKET;
    else if (c == '{')
        return cLCURLY;
    else if (c == '}')
        return cRCURLY;

    else if (isalpha(c))
        return cLETTER;
    else if (isprint(c))
        return cPRINTABLE;

    return cUNKNOWN;

}


/*
 * Checks if a lexeme is a reserved word in the language
 */
TokenType Lexer::checkReservedWord(const string& lexeme){

    //Lookup the lexeme in the map
    auto pair = reserved.find(lexeme);

    if (pair == reserved.end()) {
        //The word was not found, it is just an identifier
        return tIDENTIFIER;
    }
    else {
        //This is a reserved word
        return pair->second;
    }

}


/*
 * Counts the number of lines in a given string.
 */
int countLines(string s){

    string s1 = s;
    int n;

    int counter = 0;

    //Loop until we have counted all the '\n' in the string
    while((n = s1.find('\n')) != string::npos) {
        counter++;
        s1 = s1.substr(n+1);
    }

    return counter;
}


/*
 * Utility function used to clear the stack.
 */
template <class T>
stack<T> clearStack(stack<T> stack) {
    while (!stack.empty()) {
        stack.pop();
    }

    return stack;
}


/*
 * Initialises the list of reserved words with their corresponding token types.
 */
void Lexer::initialiseReservedWords() {

    reserved.insert({"bool", tBOOLEAN});
    reserved.insert({"float", tFLOAT});
    reserved.insert({"int", tINTEGER});
    reserved.insert({"string", tSTRING});

    reserved.insert({"true", tTRUE});
    reserved.insert({"false", tFALSE});

    reserved.insert({"and", tAND});
    reserved.insert({"or", tOR});
    reserved.insert({"not", tNOT});

    reserved.insert({"else", tELSE});
    reserved.insert({"for", tFOR});
    reserved.insert({"if", tIF});
    reserved.insert({"let", tLET});
    reserved.insert({"print", tPRINT});
    reserved.insert({"return", tRETURN});
    reserved.insert({"while", tWHILE});

}