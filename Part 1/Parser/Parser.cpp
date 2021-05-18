#include "Parser.h"

Operator getOpType(const Token& t);
VariableType getVarType(const Token& t);
bool isType(const Token& t);
bool isRelOp(const Token& t);
bool isMultOp(const Token& t);
bool isAddOp(const Token& t);


Parser::Parser() {
    this->lexer = Lexer();
}


Parser::Parser(string* program) {
    this->lexer = Lexer(program);
    this->next = lexer.getNextToken();
    this->nextnext = lexer.getNextToken();
}



/*
 * Sets the program string to be compiled.
 */
void Parser::loadProgram(string *program) {
    this->lexer.loadProgram(program);
    this->next = lexer.getNextToken();
    this->nextnext = lexer.getNextToken();
}


/*
 * Gets the next token from the lexer.
 * Current token is removed.
 */
Token Parser::getNextToken() {
    this->next = nextnext;
    this->nextnext = lexer.getNextToken();

    return next;
}



/*
 * Parse the program. Returns a constructed syntax tree.
 */
ASTProgram* Parser::parseProgram() {
    // Program ::= {<Statement>}

    vector<ASTStatement*> statements;

    //Loop until we read the end of the program
    while(next.type != tEND) {
        statements.push_back(parseStatement());
    }

    return new ASTProgram(statements, 0);
}


/*
 * Parse Methods
 */

ASTStatement * Parser::parseStatement() {
    /* Statement ::= <VariableDecl> ';'
     *             | <Assignment> ';'
     *             | <PrintStatement> ';'
     *             | <IfStatement>
     *             | <ForStatement>
     *             | <WhileStatement>
     *             | <ReturnStatement> ';'
     *             | <FunctionDecl>
     *             | <Block>
     */

    ASTStatement* statement;

    bool requiresSemiColon = false; //True if the statement needs to be followed by ;

    //Determine the type of statement
    switch (next.type) {

        case tLET:
            statement = parseVariableDecl();
            requiresSemiColon = true;
            break;

        case tIDENTIFIER:
            statement = parseAssignment();
            requiresSemiColon = true;
            break;

        case tPRINT:
            statement = parsePrint();
            requiresSemiColon = true;
            break;

        case tIF:
            statement = parseIf();
            break;

        case tFOR:
            statement = parseFor();
            break;

        case tWHILE:
            statement = parseWhile();
            break;

        case tRETURN:
            statement = parseReturn();
            requiresSemiColon = true;
            break;

        //case Type:
        case tBOOLEAN:
        case tFLOAT:
        case tINTEGER:
        case tSTRING:
            statement = parseFunctionDecl();
            break;

        case tLCURLY:
            statement = parseBlock();
            break;

        default:
            //Unrecognised
            throw runtime_error("Line " + to_string(next.lineNum) + ": Unexpected " + next.getTypeName());

    }


    //Check if we need to parse ;
    if (requiresSemiColon) {

        // If ; is missing, throw an error
        if (next.type != tSEMICOLON) {
            throw runtime_error("Line " + to_string(next.lineNum) + ": Expected ';', found " + next.getTypeName());
        }

        getNextToken();
    }


    return statement;
}


ASTExpression * Parser::parseFactor() {
    /* Factor ::= <Literal>
     *          | <Identifier>
     *          | <FunctionCall>
     *          | <SubExpression>
     *          | <Unary>
     */

    ASTExpression* factor;

    switch(next.type) {

        //case Literal:
        case tTRUE:
        case tFALSE:
        case tFLOATLITERAL:
        case tINTEGERLITERAL:
        case tSTRINGLITERAL:
            factor = parseLiteral();
            break;

        case tIDENTIFIER:
            //Check if it is a function call or just an identifier

            if (nextnext.type == tLBRACKET) {
                //Function call
                factor = parseFunctionCall();
            }
            else {
                //Identifier
                factor = parseIdentifier();
            }
            break;

        case tLBRACKET:
            factor = parseSubExpression();
            break;

        case tMINUS:
        case tNOT:
            factor = parseUnary();
            break;

        default:
            //Unrecognised
            throw runtime_error("Line " + to_string(next.lineNum) + ": Unexpected " + next.getTypeName());

    }


    return factor;
}


ASTLiteral * Parser::parseLiteral() {
    /* Literal ::= <BooleanLiteral>
     *           | <FloatLiteral>
     *           | <IntegerLiteral>
     *           | <StringLiteral>
     */

    ASTLiteral * literal;
    string tokenValue = next.value;
    int lineNum = next.lineNum;

    //Determine the literal type and convert the value if necessary
    switch (next.type) {

        case tTRUE:
            literal = new ASTLiteralBool(true, lineNum);
            break;

        case tFALSE:
            literal = new ASTLiteralBool(false, lineNum);
            break;

        case tFLOATLITERAL:
            literal = new ASTLiteralFloat(stof(tokenValue), lineNum);
            break;

        case tINTEGERLITERAL:
            literal = new ASTLiteralInt(stoi(tokenValue), lineNum);
            break;

        case tSTRINGLITERAL:
            literal = new ASTLiteralString(tokenValue.substr(1, tokenValue.size()-2), lineNum);
            break;

        default:
            //Unrecognised
            throw runtime_error("Line " + to_string(next.lineNum) + ": Expected Literal, found " + next.getTypeName());

    }
    getNextToken();

    return literal;
}


ASTAssignment * Parser::parseAssignment() {
    // <Identifier> '=' <Expression>

    ASTAssignment* assignment;
    int lineNum = next.lineNum;

    ASTIdentifier* identifier = parseIdentifier();

    // Parse =
    if (next.type == tEQUALASSIGN) {

        getNextToken();
        ASTExpression* value = parseExpression();

        assignment = new ASTAssignment(identifier, value, lineNum);

    }
    else {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected '=', found " + next.getTypeName());
    }


    return assignment;
}


ASTBlock * Parser::parseBlock() {
    // '{' {<Statement>} '}'

    ASTBlock* block;
    int lineNum = next.lineNum;

    //Parse {
    if (next.type == tLCURLY) {

        getNextToken();
        vector<ASTStatement*> statements;

        //Loop until we reach the end of the block, parse each statement
        while(next.type != tRCURLY && next.type != tEND) {
            statements.push_back(parseStatement());
        }



        //Parse }
        if (next.type == tRCURLY) {

            getNextToken();
            block = new ASTBlock(statements, lineNum);

        }
        else {
            throw runtime_error("Line " + to_string(next.lineNum) + ": Expected '}', found " + next.toString());
        }

    }
    else {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected '{', found " + next.toString());
    }

    return block;
}


ASTExpression * Parser::parseExpression() {
    // <SimpleExpression> [ <RelationalOp> <Expression> ]

    ASTExpression* expression;

    ASTExpression* lExpression;
    Operator relOp;
    ASTExpression* rExpression;
    int lineNum = next.lineNum;


    //Parse SimpleExpression
    lExpression = parseSimpleExpression();


    //Check if there is another operator
    if(isRelOp(next)) {

        //Read the Op
        relOp = getOpType(next);
        getNextToken();

        //Read the expression
        rExpression = parseExpression();


        //Return the binary op
        expression = new ASTBinOp(lExpression, relOp, rExpression, lineNum);

    }
    else {
        //It is just the one expression
        expression = lExpression;
    }

    return expression;
}


ASTFor * Parser::parseFor() {
    // 'for' '(' [<VariableDecl>] ';' <Expression> ';' [<Assignment>] ')' <Block>

    ASTVariableDecl* declaration;
    ASTExpression* conditional;
    ASTAssignment* assignment;
    ASTBlock* block;
    int lineNum = next.lineNum;


    //Parse for
    if (next.type != tFOR) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected 'for', found " + next.getTypeName());
    }
    getNextToken();

    //Parse (
    if (next.type != tLBRACKET) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected 'for', found " + next.getTypeName());
    }
    getNextToken();

    //Check if there is a variable declaration
    if (next.type == tLET) {
        declaration = parseVariableDecl();
    }
    else {
        declaration = nullptr;
    }


    //Parse ;
    if (next.type != tSEMICOLON) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected ';', found " + next.getTypeName());
    }
    getNextToken();


    //Parse conditional
    conditional = parseExpression();


    //Parse ;
    if (next.type != tSEMICOLON) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected ';', found " + next.getTypeName());
    }
    getNextToken();


    //Check if there is an assignment
    if (next.type == tIDENTIFIER) {
        assignment = parseAssignment();
    }
    else {
        assignment = nullptr;
    }


    //Parse )
    if (next.type != tRBRACKET) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected ')', found " + next.getTypeName());
    }
    getNextToken();


    //Parse Block
    block = parseBlock();



    return new ASTFor(declaration, conditional, assignment, block, lineNum);
}


ASTFormalParam * Parser::parseFormalParam() {
    // <Identifier> ':' <Type>

    ASTIdentifier* identifier;
    VariableType type;
    int lineNum = next.lineNum;


    //Parse Identifier
    if (next.type == tIDENTIFIER) {
        identifier = parseIdentifier();
    }
    else {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected identifier, found " + next.getTypeName());
    }


    //Parse :
    if (next.type != tCOLON) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected ':', found " + next.getTypeName());
    }
    getNextToken();


    //Parse type
    if (isType(next)) {
        type = getVarType(next);
        getNextToken();
    }
    else {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected data type, found " + next.getTypeName());
    }


    return new ASTFormalParam(identifier, type, lineNum);
}


ASTFunctionCall * Parser::parseFunctionCall() {
    // <Identifier> '(' [ <Expression> { ',' <Expression> } ] ')'

    ASTIdentifier* identifier;
    vector<ASTExpression*> params;
    int lineNum = next.lineNum;


    //Parse Identifier
    identifier = parseIdentifier();


    //Parse (
    if (next.type != tLBRACKET) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected '(', found " + next.getTypeName());
    }
    getNextToken();


    //Parse Parameters
    if (next.type != tLBRACKET) {
        //There are some parameters
        params.push_back(parseExpression());


        //Loop until we've found all the params
        while(next.type == tCOMMA) {
            getNextToken();
            params.push_back(parseExpression());
        }

    }


    //Parse )
    if (next.type != tRBRACKET) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected ')', found " + next.getTypeName());
    }
    getNextToken();



    return new ASTFunctionCall(identifier, params, lineNum);
}


ASTFunctionDecl * Parser::parseFunctionDecl() {
    // <Type> <Identifier> '(' [ <FormalParam> { ',' <FormalParam> } ] ')' <Block>

    VariableType type;
    ASTIdentifier* identifier;
    vector<ASTFormalParam*> params;
    ASTBlock* block;
    int lineNum = next.lineNum;


    //Parse type
    if(isType(next)) {
       type = getVarType(next);
       getNextToken();
    }
    else {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected type, found " + next.getTypeName());
    }


    //Parse identifier
    identifier = parseIdentifier();


    //Parse (
    if (next.type != tLBRACKET) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected '(', found " + next.getTypeName());
    }
    getNextToken();


    //Check if there are any parameters
    if (next.type != tRBRACKET) {
        //There are some parameters
        params.push_back(parseFormalParam());


        //Loop until we've found all the params
        while(next.type == tCOMMA) {
            getNextToken();
            params.push_back(parseFormalParam());
        }
    }


    //Parse )
    if (next.type != tRBRACKET) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected ')', found " + next.getTypeName());
    }
    getNextToken();


    //Parse block
    block = parseBlock();



    return new ASTFunctionDecl(type, identifier, params, block, lineNum);
}


ASTIdentifier * Parser::parseIdentifier() {
    // <Identifier>

    string identifier;
    int lineNum = next.lineNum;

    if (next.type == tIDENTIFIER) {
        identifier = next.value;
        getNextToken();
    }
    else {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected identifier, found " + next.getTypeName());
    }

    return new ASTIdentifier(identifier, lineNum);
}


ASTIf * Parser::parseIf() {
    // 'if' '(' <Expression> ')' <Block> [ 'else' <Block> ]

    ASTExpression* conditional;
    ASTBlock* ifBlock;
    ASTBlock* elseBlock;
    int lineNum = next.lineNum;


    //Parse if
    if (next.type != tIF) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected if, found " + next.getTypeName());
    }
    getNextToken();


    //Parse (
    if (next.type != tLBRACKET) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected '(', found " + next.getTypeName());
    }
    getNextToken();


    //Parse Expression
    conditional = parseExpression();


    //Parse )
    if (next.type != tRBRACKET) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected ')', found " + next.getTypeName());
    }
    getNextToken();


    //Parse Block
    ifBlock = parseBlock();


    //Check if there is an else block
    if (next.type == tELSE) {
        getNextToken();

        //There is an else block, parse it
        elseBlock = parseBlock();

    }
    else {
        elseBlock = nullptr;
    }


    return new ASTIf(conditional, ifBlock, elseBlock, lineNum);
}


ASTPrint * Parser::parsePrint() {
    // 'print' <Expression>

    ASTExpression* expression;
    int lineNum = next.lineNum;

    //Parse print
    if (next.type != tPRINT) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected print, found " + next.getTypeName());
    }
    getNextToken();


    //Parse Expression
    expression = parseExpression();


    return new ASTPrint(expression, lineNum);
}


ASTReturn * Parser::parseReturn() {
    // 'return' <Expression>

    ASTExpression* expression;
    int lineNum = next.lineNum;

    //Parse return
    if (next.type != tRETURN) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected return, found " + next.getTypeName());
    }
    getNextToken();


    //Parse Expression
    expression = parseExpression();


    return new ASTReturn(expression, lineNum);
}


ASTExpression * Parser::parseSimpleExpression() {
    // <Term> [ <AdditiveOp> <SimpleExpression> ]

    ASTExpression* expression;

    ASTExpression* lExpression;
    Operator addOp;
    ASTExpression* rExpression;
    int lineNum = next.lineNum;


    //Parse Term
    lExpression = parseTerm();


    //Check if there is another op
    if(isAddOp(next)) {
        //Parse Op
        addOp = getOpType(next);
        getNextToken();

        //Parse other expression
        rExpression = parseSimpleExpression();

        expression = new ASTBinOp(lExpression, addOp, rExpression, lineNum);
    }
    else {
        //There is only one expression
        expression = lExpression;
    }

    return expression;
}


ASTExpression * Parser::parseSubExpression() {
    // '(' <Expression> ')'

    ASTExpression* expression;
    int lineNum = next.lineNum;


    //Parse (
    if (next.type != tLBRACKET) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected '(', found " + next.getTypeName());
    }
    getNextToken();


    //Parse Expression
    expression = parseExpression();


    //Parse ;
    if (next.type != tRBRACKET) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected ')', found " + next.getTypeName());
    }
    getNextToken();



    return expression;
}


ASTExpression * Parser::parseTerm() {
    // <Factor> [ <MultiplicativeOp> <Term> ]

    ASTExpression* expression;

    ASTExpression* lExpression;
    Operator multOp;
    ASTExpression* rExpression;
    int lineNum = next.lineNum;


    //Parse factor
    lExpression = parseFactor();


    //Check if there is another op
    if(isMultOp(next)) {
        //Parse Op
        multOp = getOpType(next);
        getNextToken();

        //Parse Term
        rExpression = parseTerm();

        expression = new ASTBinOp(lExpression, multOp, rExpression, lineNum);
    }
    else {
        //There is only one expression
        expression = lExpression;
    }

    return expression;
}


ASTUnary * Parser::parseUnary() {
    // ( '-' | 'not' ) <Expression>

    Operator op;
    ASTExpression* expression;
    int lineNum = next.lineNum;


    //Parse Operator
    if(next.type == tMINUS) {
        // - operator
        getNextToken();
        op = MINUS;
    }
    else if (next.type == tNOT) {
        // not operator
        getNextToken();
        op = NOT;
    }
    else {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected operator, found " + next.getTypeName());
    }


    //Parse Expression
    expression = parseExpression();



    return new ASTUnary(op, expression, lineNum);
}


ASTVariableDecl * Parser::parseVariableDecl() {
    // 'let' <Identifier> ':' <Type> '=' <Expression>

    ASTIdentifier* identifier;
    VariableType type;
    ASTExpression* expression;
    int lineNum = next.lineNum;



    //Parse let
    if (next.type != tLET) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected let, found " + next.getTypeName());
    }
    getNextToken();


    //Parse identifier
    identifier = parseIdentifier();


    //Parse :
    if (next.type != tCOLON) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected ':', found " + next.getTypeName());
    }
    getNextToken();


    //Parse type
    if (isType(next)) {
        type = getVarType(next);
        getNextToken();
    }
    else {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected type, found " + next.getTypeName());
    }


    //Parse =
    if (next.type != tEQUALASSIGN) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected '=', found " + next.getTypeName());
    }
    getNextToken();


    //Parse Expression
    expression = parseExpression();


    return new ASTVariableDecl(identifier, type, expression, lineNum);
}


ASTWhile * Parser::parseWhile() {
    // 'while' '(' <Expression> ')' <Block>

    ASTExpression* conditional;
    ASTBlock* block;
    int lineNum = next.lineNum;


    //Parse while
    if (next.type != tWHILE) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected while, found " + next.getTypeName());
    }
    getNextToken();


    //Parse (
    if (next.type != tLBRACKET) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected '(', found " + next.getTypeName());
    }
    getNextToken();


    //Parse Expression
    conditional = parseExpression();


    //Parse )
    if (next.type != tRBRACKET) {
        throw runtime_error("Line " + to_string(next.lineNum) + ": Expected ')', found " + next.getTypeName());
    }
    getNextToken();


    //Parse block
    block = parseBlock();



    return new ASTWhile(conditional, block, lineNum);
}




/*
 * Helper Functions
 */


/*
 * Returns the type of operator stored in a token as an enum
 */
Operator getOpType(const Token& t){
    switch(t.type) {

        case tPLUS:
            return PLUS;
        case tMINUS:
            return MINUS;
        case tASTERISK:
            return MULT;
        case tFSLASH:
            return DIVIDE;

        case tAND:
            return AND;
        case tOR:
            return OR;
        case tNOT:
            return NOT;

        case tLESSTHAN:
            return LESSTHAN;
        case tLESSTHANEQUAL:
            return LESSTHANEQUAL;
        case tGREATERTHAN:
            return GREATERTHAN;
        case tGREATERTHANEQUAL:
            return GREATERTHANEQUAL;
        case tEQUALRELATIONAL:
            return EQUALS;
        case tNOTEQUAL:
            return NOTEQUALS;

        default:    //Default case should never occur
            return (Operator) NULL;
    }
}


/*
 * Returns the type of variable stored in a token as an enum
 */
VariableType getVarType(const Token& t) {
    switch(t.type) {

        case tTRUE:
        case tFALSE:
            return BOOL;

        case tFLOAT:
            return FLOAT;

        case tINTEGER:
            return INT;

        case tSTRING:
            return STRING;

        default:    //Default case should never occur
            return (VariableType) NULL;
    }
}


/*
 * Return true if the token represents a variable type, false otherwise.
 */
bool isType(const Token& t) {
    if (t.type == tBOOLEAN || t.type == tFLOAT || t.type == tINTEGER || t.type == tSTRING) {
        return true;
    }
    else {
        return false;
    }
}


/*
 * Returns true if the token is a relational operator, false otherwise
 */
bool isRelOp(const Token& t) {
    if (t.type == tLESSTHAN || t.type == tLESSTHANEQUAL || t.type == tGREATERTHAN || t.type == tGREATERTHANEQUAL
        || t.type == tEQUALRELATIONAL || t.type == tNOTEQUAL) {
        return true;
    }
    else {
        return false;
    }
}


/*
 * Returns true if the token is a multiplicative operator
 */
bool isMultOp(const Token& t) {
    if (t.type == tASTERISK || t.type == tFSLASH || t.type == tAND) {
        return true;
    }
    else {
        return false;
    }
}


/*
 * Returns true if the token is an additive operator
 */
bool isAddOp(const Token& t) {
    if (t.type == tPLUS || t.type == tMINUS || t.type == tOR) {
        return true;
    }
    else {
        return false;
    }
}


