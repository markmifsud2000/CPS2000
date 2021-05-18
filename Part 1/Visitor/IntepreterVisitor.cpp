#include <iostream>
#include "IntepreterVisitor.h"

using namespace std;


InterpreterVisitor::InterpreterVisitor() = default;


/*
 * Typecasts the last returned type to a given variable type.
 * Does nothing if the variable is already in the corrected type.
 * We assume that they types can be converted.
 */
void InterpreterVisitor::convertReturnedType(VariableType type) {

    //Check which type to convert to
    switch(type) {

        case BOOL:
            //Check which variable to convert
            switch(returnedType) {
                case FLOAT:
                    returnedBool = (bool) returnedFloat;
                    break;
                case INT:
                    returnedBool = (bool) returnedInt;
                    break;
                default:
                    break;
            }
            returnedType = BOOL;
            break;

        case FLOAT:
            //Check which variable to convert
            switch(returnedType) {
                case BOOL:
                    returnedFloat = (float) returnedBool;
                    break;
                case INT:
                    returnedFloat = (float) returnedInt;
                    break;
                default:
                    break;
            }
            returnedType = FLOAT;
            break;

        case INT:
            //Check which variable to convert
            switch(returnedType) {
                case BOOL:
                    returnedInt = (int) returnedBool;
                    break;
                case FLOAT:
                    returnedInt = (int) returnedFloat;
                    break;
                default:
                    break;
            }
            returnedType = INT;
            break;

        case STRING:
            returnedType = STRING;
            break;

        default:
            break;

    }

}


/*
 * Visit Functions
 */


void InterpreterVisitor::visit(ASTProgram* node) {

    //Enter a new scope
    table.push();

    //Execute each statement in the list
    for(ASTStatement* statement : node->program) {
        statement->accept(this);
    }

    //Exit the scope
    table.pop();

}


void InterpreterVisitor::visit(ASTAssignment* node) {

    //Evaluate the expression
    node->value->accept(this);

    //Check the variable's type and convert the returned value accordingly
    string id = node->identifier->identifier;
    VariableType type = table.getType(id);
    convertReturnedType(type);

    //Assign the value
    switch(returnedType) {
        case BOOL:
            table.assign(id, returnedBool);
            break;

        case FLOAT:
            table.assign(id, returnedFloat);
            break;

        case INT:
            table.assign(id, returnedInt);
            break;

        case STRING:
            table.assign(id, returnedString);
            break;

        default:
            //Unknown Type, cannot assign
            throw runtime_error("Line " + to_string(node->lineNum) + ": Cannot perform assignment.");
    }


}


void InterpreterVisitor::visit(ASTBinOp* node) {

    //Check if this is arithmetic or boolean
    switch(node->op) {
        case PLUS:
        case MINUS:
        case MULT:
        case DIVIDE:
        case GREATERTHAN:
        case GREATERTHANEQUAL:
        case LESSTHAN:
        case LESSTHANEQUAL:
        case EQUALS:
        case NOTEQUALS:
        {
            //Read the operands as floats
            node->lExpression->accept(this);
            convertReturnedType(FLOAT);
            float lValue = returnedFloat;

            node->rExpression->accept(this);
            convertReturnedType(FLOAT);
            float rValue = returnedFloat;

            //Evaluate
            if (node->op == PLUS) {
                returnedFloat = lValue + rValue;
            } else if (node->op == MINUS) {
                returnedFloat = lValue - rValue;
            } else if (node->op == MULT) {
                returnedFloat = lValue * rValue;
            } else if (node->op == DIVIDE) {
                returnedFloat = lValue / rValue;
            }
            else if (node->op == GREATERTHAN) {
                returnedFloat = lValue > rValue;
            }
            else if (node->op == GREATERTHANEQUAL) {
                returnedFloat = lValue >= rValue;
            }
            else if (node->op == LESSTHAN) {
                returnedFloat = lValue < rValue;
            }
            else if (node->op == LESSTHANEQUAL) {
                returnedFloat = lValue <= rValue;
            }
            else if (node->op == EQUALS) {
                returnedFloat = lValue == rValue;
            }
            else if (node->op == NOTEQUALS) {
                returnedFloat = lValue != rValue;
            }

            returnedType = FLOAT;
        }
            break;


        case AND:
        case OR:
        {
            //Read the operands as floats
            node->lExpression->accept(this);
            convertReturnedType(BOOL);
            bool lValue = returnedBool;

            node->rExpression->accept(this);
            convertReturnedType(FLOAT);
            bool rValue = returnedFloat;

            //Evaluate
            if (node->op == AND) {
                returnedBool = lValue && rValue;
            } else if (node->op == OR) {
                returnedBool = lValue || rValue;
            }

            returnedType = BOOL;
        }

            break;


        default:
            throw runtime_error("Line " + to_string(node->lineNum) + ": Unknown Operator.");

    }

}


void InterpreterVisitor::visit(ASTBlock* node) {

    //Enter a new scope
    table.push();

    //Execute each statement in the list
    for(ASTStatement* statement : node->block) {
        statement->accept(this);
    }

    //Exit the scope
    table.pop();

}


void InterpreterVisitor::visit(ASTFor* node) {

    //Enter a new scope
    table.push();

    //If there is a variable declaration, visit it
    if (node->declaration != nullptr) {
        node->declaration->accept(this);
    }


    //Check the condition
    node->conditional->accept(this);
    convertReturnedType(BOOL);

    //Loop while the conditional returns true
    while (returnedBool) {

        //Evaluate the loop block
        node->block->accept(this);

        //Evaluate the increment/assignment
        node->assignment->accept(this);

        //Re-evaluate the conditional
        node->conditional->accept(this);
        convertReturnedType(BOOL);
    }


    //Exit the scope
    table.pop();

}


void InterpreterVisitor::visit(ASTFormalParam* node) {
    //No need to do anything, formal param should not be visited
}


void InterpreterVisitor::visit(ASTFunctionCall* node) {

    //Evaluate each parameter and check its returned type
    vector<VariableType> types;
    vector<ASTLiteral*> values;
    for (ASTExpression* param : node->param) {
        param->accept(this);

        //Store the type
        types.push_back(returnedType);

        //Store the correct value
        if (returnedType == BOOL) {
            values.push_back(new ASTLiteralBool(returnedBool, 0));
        }
        else if (returnedType == FLOAT) {
            values.push_back(new ASTLiteralFloat(returnedFloat, 0));
        }
        else if (returnedType == INT) {
            values.push_back(new ASTLiteralInt(returnedInt, 0));
        }
        else if (returnedType == STRING) {
            values.push_back(new ASTLiteralString(returnedString, 0));
        }

    }

    //Find the declaration of the function being called
    string id = node->identifier->identifier;
    ASTFunctionDecl* func = table.getFunction(id, &types);


    //Enter a new scope
    table.push();

    int i = 0;
    //Declare each parameter as a variable in the scope
    for (ASTFormalParam* formalParam : func->parameters) {
        table.declare(formalParam);

        //Assign its value
        if (formalParam->type == BOOL) {
            ASTLiteralBool* literal = (ASTLiteralBool*) values[i];
            table.assign(formalParam->identifier->identifier, literal->b);

        }
        else if (formalParam->type == FLOAT) {
            ASTLiteralFloat* literal = (ASTLiteralFloat*) values[i];
            table.assign(formalParam->identifier->identifier, literal->f);
        }
        else if (formalParam->type == INT) {
            ASTLiteralInt* literal = (ASTLiteralInt*) values[i];
            table.assign(formalParam->identifier->identifier, literal->i);
        }
        else if (formalParam->type == STRING) {
            ASTLiteralString* literal = (ASTLiteralString*) values[i];
            table.assign(formalParam->identifier->identifier, literal->s);
        }

        i++;
    }


    // Execute the function
    func->block->accept(this);


    //Store the returned value in the correct type
    convertReturnedType(func->returnType);
    returnedType = func->returnType;

    //Exit the scope
    table.pop();

}


void InterpreterVisitor::visit(ASTFunctionDecl* node) {
    //Just declare the function in the symbol table
    table.declare(node);
}


void InterpreterVisitor::visit(ASTIdentifier* node) {
    //Get the literal node stored as the symbol and visit it
    table.findSymbol(node->identifier)->second.value->accept(this);
}


void InterpreterVisitor::visit(ASTIf* node) {
    //Enter a new scope
    table.push();

    //Check the condition
    node->conditional->accept(this);
    convertReturnedType(BOOL);

    //Loop while the conditional returns true
    if (returnedBool) {
        //Evaluate the conditional block
        node->ifBlock->accept(this);
    }
    else {
        //If we did not enter the if block, run the else block
        //Check if it exists
        if (node->elseBlock != nullptr) {
            node->elseBlock->accept(this);
        }
    }

    //Exit the scope
    table.pop();
}


void InterpreterVisitor::visit(ASTLiteralBool* node) {
    //Just set the value
    returnedBool = node->b;
    returnedType = BOOL;
}


void InterpreterVisitor::visit(ASTLiteralFloat* node) {
    //Just set the value
    returnedFloat = node->f;
    returnedType = FLOAT;
}


void InterpreterVisitor::visit(ASTLiteralInt* node) {
    //Just set the value
    returnedInt = node->i;
    returnedType = INT;
}


void InterpreterVisitor::visit(ASTLiteralString* node) {
    //Just set the value
    returnedString = node->s;
    returnedType = STRING;
}


void InterpreterVisitor::visit(ASTPrint* node) {
    //Evaluate the expression
    node->expression->accept(this);

    //Print the correct value
    if (returnedType == BOOL) {
        cout << (returnedBool ? "true":"false") << endl;
    }
    else if (returnedType == FLOAT) {
        cout << returnedFloat << endl;
    }
    else if (returnedType == INT) {
        cout << returnedInt << endl;
    }
    else if (returnedType == STRING) {
        cout << returnedString << endl;
    }

}


void InterpreterVisitor::visit(ASTReturn* node) {
    //Evaluate the returned expression
    node->returnValue->accept(this);
}


void InterpreterVisitor::visit(ASTUnary* node) {

    //Check the operator
    if (node->op == MINUS) {
        //Convert to a float and then carry out the operation
        convertReturnedType(FLOAT);
        returnedFloat = -returnedFloat;
    }
    else if (node-> op == NOT) {
        //Convert to a bool and then carry out the operation
        convertReturnedType(BOOL);
        returnedBool = !returnedBool;
    }
    else {
        throw runtime_error("Line " + to_string(node->lineNum) + ": Unknown Operator.");
    }

}


void InterpreterVisitor::visit(ASTVariableDecl* node) {
    //First declare the variable
    table.declare(node);

    //Then initialise it using an assignment
    node->value->accept(this);

    //Assign the correct value
    string id = node->identifier->identifier;
    if(returnedType == BOOL) {
        table.assign(id, returnedBool);
    }
    else if(returnedType == FLOAT) {
        table.assign(id, returnedFloat);
    }
    else if(returnedType == INT) {
        table.assign(id, returnedInt);
    }
    else if(returnedType == STRING) {
        table.assign(id, returnedString);
    }
}


void InterpreterVisitor::visit(ASTWhile* node) {

    //Enter a new scope
    table.push();

    //Check the condition
    node->conditional->accept(this);
    convertReturnedType(BOOL);

    //Loop while the conditional returns true
    while (returnedBool) {

        //Evaluate the loop block
        node->block->accept(this);

        //Re-evaluate the conditional
        node->conditional->accept(this);
        convertReturnedType(BOOL);
    }

    //Exit the scope
    table.pop();

}

