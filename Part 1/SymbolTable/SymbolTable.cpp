#include "SymbolTable.h"

SymbolTable::SymbolTable() = default;


/*
 * Stack Functions
 */


/*
 * Create a new scope on the stack.
 */
void SymbolTable::push() {
    Scope s;
    stack.push_back(s);
}

/*
 * Remove the innermost scope from the stack.
 */
void SymbolTable::pop() {
    stack.pop_back();
}

/*
 * Returns a pointer to the innermost Scope
 */
Scope* SymbolTable::top() {
    return &*(stack.end()-1);
}



/*
 * Symbol Table Functions
 */

/*
 * Checks if a variable is declared in any scope
 */
bool SymbolTable::isDeclared(const string& id) {

    //Look through each scope, starting with the innermost
    auto i = stack.rbegin(); //Start from the last added scope


    while (i != stack.rend()) {
        //Loop through every scope

        if (i->find(id) != i->end()) {
            //We have found it
            return true;
        }

        //Check the next (larger/outer) scope
        i++;
    }

    //It was not found
    return false;
}



/*
 * Checks if a function is declared in any scope
 */
bool SymbolTable::isDeclared(const string& id, vector<VariableType>* types) {

    //Look through each scope, starting with the innermost
    auto i = stack.rbegin(); //Start from the last added scope


    while (i != stack.rend()) {
        //Loop through every scope for the identifier

        if (i->find(id) != i->end()) {
            //We have found it
            //Check the parameter list

            Symbol func = (i->find(id))->second;
            for (ASTFunctionDecl* ast : func.func) {
                //Check each declaration of the function
                bool output = true;

                int j = 0;
                //Check each of the params to see if they match
                for (ASTFormalParam* param : ast->parameters) {

                    if (param->type != (*types)[j]) {
                        // Params do not match
                        output = false;
                        break;
                    }

                   j++;
                }

                // If we found the matching params, just return
                if (output) {
                    return true;
                }
                // Otherwise, check the next parameter set
            }

            //Matching parameters were not found
            return false;
        }

        i++;
    }

    //Identifier was not found
    return false;
}



/*
 * Declare a variable in the symbol table.
 * If there already exists a variable with the same name in the current scope, throw an error.
 * We can ignore outer scopes.
 */
void SymbolTable::declare(ASTVariableDecl* node) {

    string id = node->identifier->identifier; //The identifier that needs to be declared

    //Check if it is already declared in the current scope
    if (isDeclaredScope(id)) {
        //It is already declared, throw an error
        throw runtime_error("Line " + to_string(node->lineNum) + ": Variable " + id + " is already declared in the current scope.");
    }

    else {
        //It is not declared, add it to the scope's symbols
        Symbol s = {.type=node->type, .lineNum=node->lineNum, .func={}, .value=nullptr};
        top()->insert({id, s});

    }

}


/*
 * Declare a function in the symbol table.
 * Functions can be overloaded.
 * Overloaded functions must have the same return type.
 * Since we assume that functions can only be declared in the global scope,
 * we can also assume that top() is the global (and only) scope.
 */
void SymbolTable::declare(ASTFunctionDecl* node) {

    string id = node->identifier->identifier; //The identifier that needs to be declared
    vector<VariableType> paramTypes;          //The types of parameters accepted by the function

    //Convert the param list to a VariableType list
    for (ASTFormalParam* ast : node->parameters) {
        paramTypes.push_back(ast->type);
    }


    //Check if the function is already declared in the program
    if (isDeclared(id, &paramTypes)) {
        //It is already declared, throw an error

        //Create the error message
        string error = "Line " + to_string(node->lineNum) + ": Function " + id + "(";
        for (int i = 0; i < paramTypes.size();) {
            //List the param type
            error += typeToString(paramTypes[i]);

            //Only add a comma if there is another param afterwards
            if (++i < paramTypes.size()){
                error += ", ";
            }
        }
        error += ") is already declared.";

        throw runtime_error(error);
    }

    else {
        //It is not declared, check if we are overloading

        if (isDeclared(id)) {
            //We are just overloading, check if return types are the same.

            Symbol* s = &(top()->find(id)->second);
            if (s->type != node->returnType) {
                //They have different return types, throw an error
                throw runtime_error("Line " + to_string(node->lineNum) + ": Function " + id + " has already been declared with a different return type.");
            }
            else {
                //They have the same return types, just add it to the list of functions
                s->func.push_back(node);
            }

        }

        else {
            //We are not overloading, create a new symbol
            vector<ASTFunctionDecl*> func;
            func.push_back(node);

            Symbol s = {.type=node->returnType, .lineNum=node->lineNum, .func=func};

            //Add the symbol to the table
            top()->insert({id, s});
        }


    }

}


/*
 * Take a formal parameter and declare it as a variable in the scope
 */
void SymbolTable::declare(ASTFormalParam* node) {
    auto declaration = new ASTVariableDecl(node->identifier, node->type, nullptr, node->lineNum);
    declare(declaration);
    delete declaration;
}


/*
 * Assign a new value to a bool variable in the table
 */
void SymbolTable::assign(const string& id, bool value) {
    findSymbol(id)->second.value = new ASTLiteralBool(value, 0);
}


/*
 * Assign a new value to a float variable in the table
 */
void SymbolTable::assign(const string& id, float value) {
    findSymbol(id)->second.value = new ASTLiteralFloat(value, 0);
}


/*
 * Assign a new value to a int variable in the table
 */
void SymbolTable::assign(const string& id, int value) {
    findSymbol(id)->second.value = new ASTLiteralInt(value, 0);
}


/*
 * Assign a new value to a string variable in the table
 */
void SymbolTable::assign(const string& id, const string& value) {
    findSymbol(id)->second.value = new ASTLiteralString(value, 0);
}


/*
 * Checks the type of a symbol in the table.
 * We assume that the symbol is actually declared.
 */
VariableType SymbolTable::getType(const string& id) {
    return findSymbol(id)->second.type;
}


/*
 * Finds the function declaration with matching identifier and parameters.
 * We assume that the symbol is actually declared.
 */
ASTFunctionDecl * SymbolTable::getFunction(const string &s, vector<VariableType>* types) {

    //Find the function with matching identifier
    vector<ASTFunctionDecl*> functions = findSymbol(s)->second.func;

    //Check each function declaration to find the corresponding parameter list
    for (ASTFunctionDecl* f : functions) {

        bool isFound = true;
        int i = 0;

        //Check if the parameter list matches
        for (ASTFormalParam* param : f->parameters) {

            if (param->type != (*types)[i]) {
                //Params don't match
                isFound = false;
                break;
            }

            i++;
        }

        //Check if the params match
        if(isFound) {
            //They match, this is the required function
            return f;
        }

    }

    return nullptr;
}


/*
 * Check if a variable is declared in the current scope.
 */
bool SymbolTable::isDeclaredScope(const string& id) {

    if (top()->find(id) == top()->end()) {
        //It is not in the table
        return false;
    }
    else {
        //It is in the scope
        return true;
    }


}


/*
 * Finds a key & value pair int the symbol table.
 * We assume that the symbol is actually declared.
 */
map<string, Symbol>::iterator SymbolTable::findSymbol(const string& id) {

    //Look through each scope, starting with the innermost
    auto i = stack.rbegin(); //Start from the last added scope


    while (i != stack.rend()) {
        //Loop through every scope

        auto value = i->find(id);
        if (value != i->end()) {
            //We have found it
            return value;
        }

        //Check the next (larger/outer) scope
        i++;
    }

    //It was not found
    return i->end();
}


