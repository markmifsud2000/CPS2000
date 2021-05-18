#ifndef CPS2000_ASSIGNMENT_SYMBOLTABLE_H
#define CPS2000_ASSIGNMENT_SYMBOLTABLE_H

#include <stack>
#include <map>
#include <stdexcept>

#include "../AST/AST.h"


using namespace std;



/*
 * Used to store both variable types and their value in the lookup table
 */
struct Symbol {

    VariableType type;              //The type of identifier stored
    int lineNum;                    //Used for reporting errors

    //Stores any functions with the same identifier
    vector<ASTFunctionDecl*> func;

    //Expression evaluates to the initial value of the variable
    ASTLiteral* value;
};


typedef map<string, Symbol> Scope;  //All the symbols in the current scope


/*
 * The SymbolTable class actually represents a stack of symbol tables.
 * The bottom table represents the outermost (global) scope,
 * while the top table represents the innermost (current) scope,
 */
class SymbolTable {
public:
    SymbolTable();

    //Symbol Table Functions

    bool isDeclared(const string& id);
    bool isDeclared(const string& id, vector<VariableType>* types);

    void declare(ASTVariableDecl* node);
    void declare(ASTFunctionDecl* node);
    void declare(ASTFormalParam* node);

    VariableType getType(const string& s);
    ASTFunctionDecl* getFunction(const string& s, vector<VariableType>* types);
    map<string, Symbol>::iterator findSymbol(const string& id);

    void assign(const string& id, bool value);
    void assign(const string& id, float value);
    void assign(const string& id, int value);
    void assign(const string& id, const string& value);


    //Stack Functions

    void push();
    void pop();
    Scope* top();



private:
    vector<Scope> stack;      //The stack of scopes

    bool isDeclaredScope(const string& id);
};


#endif //CPS2000_ASSIGNMENT_SYMBOLTABLE_H
