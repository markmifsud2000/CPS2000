#include "SemanticVisitor.h"

bool doTypesMatch(VariableType expected, VariableType actual);
VariableType opReturnType(VariableType lType, Operator op, VariableType rType);


SemanticVisitor::SemanticVisitor() = default;

/*
 * Checks whether two variable types are the same.
 * Also checks if the actual type found can be automatically cast to the expected type.
 */
bool doTypesMatch(VariableType expected, VariableType actual) {

    switch (expected) {

        case BOOL:
        case FLOAT:
        case INT:
            // We can convert between bool, float and int
            switch (actual) {
                case BOOL:
                case FLOAT:
                case INT:
                    return true;

                case STRING:
                default:
                    return false;
            }



        case STRING:
            // String can only accept a string
            switch (actual) {
                case STRING:
                    return true;

                case BOOL:
                case FLOAT:
                case INT:
                default:
                    return false;
            }


        default:
            return false;

    }

}


/*
 * Checks whether the types given are compatible with an operator.
 * Returns the type of the value produced by the operation.
 * Returns INCOMPATIBLE if the types are not compatible.
 */
VariableType opReturnType(VariableType lType, Operator op, VariableType rType) {

    switch(op) {

        case MULT:
        case DIVIDE:
            //Multiplicative Op, we will carry out the actual operation with floats,
            //we can then convert back to the appropriate type (eg. int) later
            if (doTypesMatch(FLOAT, lType) && doTypesMatch(FLOAT, rType)){
                return FLOAT;
            }
            break;


        case PLUS:
        case MINUS:
            //Additive op, Both types must be compatible
            if (doTypesMatch(lType, rType)) {
                //The right type can be converted to the left type, the resulting value can be lType
                return lType;
            }
            if (doTypesMatch(rType, lType)) {
                //The right type can be converted to the right type, the resulting value can be the rType
                return rType;
            }
            break;


        case LESSTHAN:
        case LESSTHANEQUAL:
        case GREATERTHAN:
        case GREATERTHANEQUAL:
        case EQUALS:
        case NOTEQUALS:
            //Comparison Operator, both types must be compatible
            if (doTypesMatch(lType, rType) || doTypesMatch(rType, lType)) {
                return BOOL;
            }
            break;


        case AND:
        case OR:
            //Boolean Operations, both operands must be bools
            if (doTypesMatch(BOOL, lType) && doTypesMatch(BOOL, rType)){
                return BOOL;
            }
            break;


        default:
            return INCOMPATIBLE;

    }

    //If we have not yet returned, it must be incompatible
    return INCOMPATIBLE;
}


/*
 * Visit Functions
 */


void SemanticVisitor::visit(ASTProgram *node) {

    //Enter the global scope
    table.push();

    //Check each statement
    for (ASTStatement* statement : node->program) {
        statement->accept(this);
    }

    //Exit the scope
    table.pop();
}


void SemanticVisitor::visit(ASTAssignment *node) {

    //Check if the variable is already declared

    string id = node->identifier->identifier;
    if (!table.isDeclared(id)) {
        //Variable has not been declared, cannot assign.
        throw runtime_error("Line " + to_string(node->lineNum) + ": Variable " + id + " has not been declared.");
    }

    //Check if the type of the variable matches the type returned by the expression
    VariableType varType = table.getType(id);

    //Check if the expression is semantically valid and what type it returns
    node->value->accept(this);

    //Check if the expression's type matches the variable's type
    if (!doTypesMatch(varType, returnedType)) {
        //Types do not match, throw error
        throw runtime_error("Line " + to_string(node->lineNum) + ": Variable " + id + " is of type " +
                                    typeToString(varType) + " but found " + typeToString(returnedType) + ".");
    }

    //Update returned type
    returnedType = varType;

}


void SemanticVisitor::visit(ASTBinOp *node) {

    //Check the left expression and get its return type
    node->lExpression->accept(this);
    VariableType lType = returnedType;

    //Check the right expression and get its return type
    node->rExpression->accept(this);
    VariableType rType = returnedType;

    //Check if both types are compatible under the operation
    //and get the type returned
    returnedType = opReturnType(lType, node->op, rType);

    //If incompatible, throw error
    if (returnedType == INCOMPATIBLE) {
        throw runtime_error("Line " + to_string(node->lineNum) + ": Types " + typeToString(lType) + " and " +
                                    typeToString(rType) + " are not compatible under this operation.");
    }

}


void SemanticVisitor::visit(ASTBlock *node) {

    //Enter a new Scope
    table.push();

    //Check each statement in the block
    for (ASTStatement* statement : node->block) {
        statement->accept(this);
    }

    //Exit the scope
    table.pop();

}


void SemanticVisitor::visit(ASTFor *node) {

    //Enter a new scope
    table.push();

    //If a variable declaration is made, check it
    if (node->declaration != nullptr) {
        node->declaration->accept(this);
    }

    //Check the conditional expression returns a boolean value
    node->conditional->accept(this);
    if (returnedType != BOOL) {
        //It is not a bool, throw an error
        throw runtime_error("Line " + to_string(node->conditional->lineNum)
                                + ": Condition inside for loop must return type bool, instead found type " +
                                    typeToString(returnedType));
    }

    //If a assignment exists, check it
    if(node->assignment != nullptr) {
        node->assignment->accept(this);
    }


    //Check the loop's main block
    node->block->accept(this);


    //Exit the scope
    table.pop();

}


void SemanticVisitor::visit(ASTFormalParam *node) {
    //Just declare the parameter
    table.declare(node);
}


void SemanticVisitor::visit(ASTFunctionCall *node) {

    //Convert the parameter list to a type list
    vector<VariableType> types;
    for (ASTExpression* param : node->param) {
        //Check the parameter expression
        param->accept(this);
        types.push_back(returnedType);
    }

    //Check if the function is declared and if the parameters match
    string id = node->identifier->identifier;

    if (!table.isDeclared(id, &types)) {
        //The function does not exist, throw an error
        string error = "Line " + to_string(node->lineNum) + ": Function " + id + "(";

        for (int i=0; i<types.size();) {
            error += typeToString(types[i]);
            if (++i < types.size()) {
                error += ", ";
            }
        }

        error += ") is not defined.";
        throw runtime_error(error);
    }

    //check the type returned by the function
    returnedType = table.getType(id);
}


void SemanticVisitor::visit(ASTFunctionDecl *node) {
    //Enter a new Scope
    table.push();

    //Declare the parameters, also store their types for declaring the function later
    for (ASTFormalParam* param : node->parameters) {
        param->accept(this);
    }


    //We will check the block manually rather than visiting it
    //This allows us to ensure there is a return statement


    auto block = node->block->block;
    for (int i = 0; i < block.size()-1; i++) {
        block[i]->accept(this);
    }

    //Check the last statement separately to make sure it is a return statement
    returnStatement = false;
    block.back()->accept(this);

    if (!returnStatement) {
        //Return statement is missing, throw an error.
        throw runtime_error("Line " + to_string(node->lineNum) + ": Missing return statement.");
    }

    //Check the type returned by the return statement matches the function's return type
    if (!doTypesMatch(node->returnType, returnedType)) {
        //Types do not match, throw error
        throw runtime_error("Line " + to_string(node->lineNum) + ": Return type does not match, expected " +
                                    typeToString(node->returnType) + ", found " + typeToString(returnedType) + ".");
    }



    //Exit the Scope
    table.pop();


    //Declare the function
    table.declare(node);

    //Set the return type
    returnedType = node->returnType;


}


void SemanticVisitor::visit(ASTIdentifier *node) {

    //Check if the identifier is declared
    if (!table.isDeclared(node->identifier)) {
        //identifier is not declared, throw an error
        throw runtime_error("Line " + to_string(node->lineNum) + ": Variable " + node->identifier
                                + " has not been declared.");
    }

    //Check it's type
    returnedType = table.getType(node->identifier);
}


void SemanticVisitor::visit(ASTIf *node) {

    //Check that the conditional statement is valid and returns a bool
    node->conditional->accept(this);

    if (!doTypesMatch(BOOL, returnedType)) {
        //Condition does not return a bool
        throw runtime_error("Line " + to_string(node->lineNum) + ": Expected type bool, found type " +
                                    typeToString(returnedType) + ".");
    }

    //Visit the main block
    node->ifBlock->accept(this);


    //If there is an else block, visit that too
    if (node->elseBlock != nullptr) {
        node->elseBlock->accept(this);
    }


}


void SemanticVisitor::visit(ASTLiteralBool *node) {
    //Just set the returned type
    returnedType = BOOL;
}


void SemanticVisitor::visit(ASTLiteralFloat *node) {
    //Just set the returned type
    returnedType = FLOAT;
}


void SemanticVisitor::visit(ASTLiteralInt *node) {
    //Just set the returned type
    returnedType = INT;
}


void SemanticVisitor::visit(ASTLiteralString *node) {
    //Just set the returned type
    returnedType = STRING;
}


void SemanticVisitor::visit(ASTPrint *node) {
    //Check the expression being printed
    node->expression->accept(this);
}


void SemanticVisitor::visit(ASTReturn *node) {
    //Check the expression being returned
    node->returnValue->accept(this);
    returnStatement = true;
}


void SemanticVisitor::visit(ASTUnary *node) {

    //Check the expression
    node->expression->accept(this);

    //Check if the type of the expression is compatible with the op
    switch (node->op) {

        case MINUS:
            switch (returnedType) {
                case INT:
                case FLOAT:
                    //Can be negated, do nothing
                    break;

                default:
                    //Cannot negate
                    throw runtime_error("Line " + to_string(node->lineNum) + ": Type " + typeToString(returnedType)
                                            + " is not compatible with operator -.");
            }
            break;


        case NOT:
            switch (returnedType) {
                case BOOL:
                    //Can be negated, do nothing
                    break;

                default:
                    //Cannot negate
                    throw runtime_error("Line " + to_string(node->lineNum) + ": Type " + typeToString(returnedType)
                                        + " is not compatible with operator not.");
            }
            break;


        default:
            //This case should not be possible as it is already checked by the parser
            throw runtime_error("Line " + to_string(node->lineNum) + ": Operator requires two parameters.");

    }

}


void SemanticVisitor::visit(ASTVariableDecl *node) {

    //Check that the value assigned is valid
    node->value->accept(this);

    //Check that the returned type of the expression matches the type of the variable
    if (!doTypesMatch(node->type, returnedType)) {
        //Types are not compatible, throw an error
        throw runtime_error("Line " + to_string(node->lineNum) + ": Cannot assign value of type "
                                + typeToString(returnedType) + " to variable of type "
                                + typeToString(node->type) + ".");
    }


    //Declare the variable in the scope
    table.declare(node);

}


void SemanticVisitor::visit(ASTWhile *node) {

    //Check that the conditional statement is valid and returns a bool
    node->conditional->accept(this);

    if (!doTypesMatch(BOOL, returnedType)) {
        //Condition does not return a bool
        throw runtime_error("Line " + to_string(node->lineNum) + ": Expected type bool, found type " +
                            typeToString(returnedType) + ".");
    }

    //Visit the loop block
    node->block->accept(this);

}

