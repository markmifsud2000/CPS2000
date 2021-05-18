#include "XMLVisitor.h"
#include "../AST/AST.h"

//Used to convert enum to string for output
const string typeString[] = {"bool", "float", "int", "string"};
const string opString[] = {"*", "/", "+", "-", "<", "<=", ">", ">=", "==", "!=", "and", "or", "not"};


/*
 * Constructors open the output file.
 */

XMLVisitor::XMLVisitor() {
    //Open File
    this->file.open(DEFAULT_FILENAME);
    this->indent = 0;
}

XMLVisitor::XMLVisitor(const string& filename) {
    //Open File
    this->file.open(filename);
    this->indent = 0;
}

XMLVisitor::~XMLVisitor() {
    //Close file
    this->file.close();
}


/*
 * Adds indentation at the start of the line
 */
void XMLVisitor::addIndent() {
    for (int i = 0; i < indent; i++) {
        file << "\t";
    }
}


/*
 * Visit functions.
 */

void XMLVisitor::visit(ASTProgram* node) {
    //Open program block
    addIndent();
    file << "<Program>" << endl;
    indent++;

    //Visit each statement
    for (ASTStatement* s : node->program) {
        s->accept(this);
    }

    //Close program block
    indent--;
    addIndent();
    file << "</Program>" << endl;
}


void XMLVisitor::visit(ASTAssignment* node) {
    //Open assignment Block
    addIndent();
    file << "<Assignment>" << endl;
    indent++;

    //Print values
    node->identifier->accept(this);
    node->value->accept(this);

    //Close assignment block
    indent--;
    addIndent();
    file << "</Assignment>" << endl;
}


void XMLVisitor::visit(ASTBinOp* node) {
    //Open binary op block
    addIndent();
    file << "<BinOp op=\"" << opString[node->op] << "\">" << endl;
    indent++;

    //visit operands
    node->lExpression->accept(this);
    node->rExpression->accept(this);

    //Close binary op block
    indent--;
    addIndent();
    file << "</BinOp>" << endl;
}


void XMLVisitor::visit(ASTBlock* node) {
    //Open block block
    addIndent();
    file << "<Block>" << endl;
    indent++;

    //Visit each statement in the block
    for (ASTStatement* s : node->block) {
        s->accept(this);
    }

    //Close block block
    indent--;
    addIndent();
    file << "</Block>" << endl;
}


void XMLVisitor::visit(ASTFor* node) {
    //Open for block
    addIndent();
    file << "<for>" << endl;
    indent++;

    //Visit assignment/conditional/assignment (if they exist)
    addIndent();
    file << "<Condition>" << endl;
    indent++;

    if (node->declaration != nullptr) {
        node->declaration->accept(this);
    }

    node->conditional->accept(this);

    if (node->assignment != nullptr) {
        node->assignment->accept(this);
    }

    indent--;
    addIndent();
    file << "</Condition>" << endl;


    //Print main block
    addIndent();
    file << "<Do>" << endl;
    indent++;

    node->block->accept(this);

    indent--;
    addIndent();
    file << "</Do>" << endl;


    //Close for block
    indent--;
    addIndent();
    file << "</Assignment>" << endl;
}


void XMLVisitor::visit(ASTFormalParam* node) {
    //Open param line
    addIndent();
    file << "<Param type=\"" << typeString[node->type] << "\">" << endl;
    indent++;

    //Print id
    node->identifier->accept(this);

    //Close param line
    indent--;
    addIndent();
    file << "</Param>" << endl;
}


void XMLVisitor::visit(ASTFunctionCall* node) {
    //Open assignment block
    addIndent();
    file << "<Function>" << endl;
    indent++;

    //Print function name
    node->identifier->accept(this);

    //Open Param Block
    addIndent();
    file << "<Params>" << endl;
    indent++;

    //Print all params
    for (ASTExpression* e : node->param) {
        e->accept(this);
    }

    //Close assignment block
    indent--;
    addIndent();
    file << "</Params>" << endl;

    //Close function block
    indent--;
    addIndent();
    file << "</Function>" << endl;
}


void XMLVisitor::visit(ASTFunctionDecl* node) {
    //Open function block
    addIndent();
    file << "<Function return=\"" << typeString[node->returnType] << "\">" << endl;
    indent++;

    //Print id
    node->identifier->accept(this);

    //Open Param List
    addIndent();
    file << "<Params>" << endl;
    indent++;

    //Print each param
    for (ASTFormalParam* p : node->parameters) {
        p->accept(this);
    }

    //End Param list
    indent--;
    addIndent();
    file << "</Params>" << endl;

    //Print block
    node->block->accept(this);

    //Close function line
    indent--;
    addIndent();
    file << "</Function>" << endl;
}


void XMLVisitor::visit(ASTIdentifier* node) {
    addIndent();
    file << "<Id>" << node->identifier << "</Id>" << endl;
}


void XMLVisitor::visit(ASTIf* node) {
    //Open if block
    addIndent();
    file << "<If>" << endl;
    indent++;

    //Print the conditional statement
    addIndent();
    file << "<Condition>" << endl;
    indent++;

    node->conditional->accept(this);

    indent--;
    addIndent();
    file << "</Condition>" << endl;

    //Print the main block
    node->ifBlock->accept(this);


    //If the else block exists, print it
    if (node->elseBlock != nullptr) {
        //Print the conditional statement
        addIndent();
        file << "<Else>" << endl;
        indent++;

        node->elseBlock->accept(this);

        indent--;
        addIndent();
        file << "</Else>" << endl;
    }


    //Close the if block
    indent--;
    addIndent();
    file << "</If>" << endl;
}


void XMLVisitor::visit(ASTLiteralBool* node) {
    //Convert bool value
    string value = (node->b) ? "true" : "false";

    //Open Literal Line
    addIndent();
    file << "<Literal type=\"bool\">" << value << "</Literal>" << endl;
}


void XMLVisitor::visit(ASTLiteralFloat* node) {
    //Open Literal Line
    addIndent();
    file << "<Literal type=\"float\">" << to_string(node->f) << "</Literal>" << endl;
}


void XMLVisitor::visit(ASTLiteralInt* node) {
    //Open Literal Line
    addIndent();
    file << "<Literal type=\"int\">" << to_string(node->i) << "</Literal>" << endl;
}


void XMLVisitor::visit(ASTLiteralString* node) {
    //Open Literal Line
    addIndent();
    file << "<Literal type=\"string\">" << node->s << "</Literal>" << endl;
}


void XMLVisitor::visit(ASTPrint* node) {
    //Open print block
    addIndent();
    file << "<print>" << endl;
    indent++;

    //Print Expression
    node->expression->accept(this);

    //Close print block
    indent--;
    addIndent();
    file << "</print>" << endl;
}


void XMLVisitor::visit(ASTReturn* node) {
    //Open return block
    addIndent();
    file << "<return>" << endl;
    indent++;

    //Print Expression
    node->returnValue->accept(this);

    //Close return block
    indent--;
    addIndent();
    file << "</return>" << endl;
}


void XMLVisitor::visit(ASTUnary* node) {
    //Open Unary Block
    addIndent();
    file << "<UnaryOp op=\"" << opString[node->op] << "\">" << endl;
    indent++;

    //Print the expression
    node->expression->accept(this);

    //Close Unary block
    indent--;
    addIndent();
    file << "</UnaryOp>" << endl;
}


void XMLVisitor::visit(ASTVariableDecl* node) {
    //Open declaration block
    addIndent();
    file << "<VariableDecl type=\"" << typeString[node->type] << "\">" << endl;
    indent++;

    //Print the identifier
    node->identifier->accept(this);

    //Print the value
    node->value->accept(this);

    //Close the declaration block
    indent--;
    addIndent();
    file << "</VariableDecl>" << endl;
}


void XMLVisitor::visit(ASTWhile* node) {
    //Open while block
    addIndent();
    file << "<while>" << endl;
    indent++;

    //Print the conditional
    addIndent();
    file << "<Condition>" << endl;
    indent++;

    node->conditional->accept(this);

    indent--;
    addIndent();
    file << "</Condition>" << endl;

    //Print the block
    addIndent();
    file << "<Do>" << endl;
    indent++;

    node->block->accept(this);

    indent--;
    addIndent();
    file << "</Do>" << endl;

    //Close the while block
    indent--;
    addIndent();
    file << "</while>" << endl;
}

