#include <iostream>
#include <cerrno>
#include <string>

#include "./Visitor/IntepreterVisitor.h"
#include "./Visitor/SemanticVisitor.h"
#include "./Visitor/XMLVisitor.h"
#include "./Parser/Parser.h"


using namespace std;


string readFile(char* fileName);



/*
 * Expected Arguments: File name
 */
int main(int argc, char** argv) {

    //Check argument list size
    if (argc > 2) {
        cerr << "Too Many Arguments" << endl;
        exit(E2BIG);
    }
    else  if (argc < 2) {
        return 0;
    }


    //Read the file
    string program = readFile(argv[1]);


    Parser p = Parser(&program);
    ASTProgram* node = p.parseProgram();


    auto xml = new XMLVisitor();
    auto semantic = new SemanticVisitor();
    auto interpreter = new InterpreterVisitor();


    node->accept(xml);
    node->accept(semantic);
    node->accept(interpreter);



//     Lexer
    /*Lexer l(&program);
    Token t = l.getNextToken();
    while (t.type != tEND && t.type != tREJECTED) {
        cout << t.toString() << endl;
        t = l.getNextToken();
    }*/




    return 0;
}


/*
 * Read a file as a string.
 */
string readFile(char* fileName) {

    fstream f(fileName);

    //Check if file was opened
    if (!f.is_open()) {
        cerr << "File could not be opened" << endl;
        exit(EBADF);
    }

    string program;
    string tmp;
    //Read each line from the file and append it to the string
    while (getline(f, tmp)) {
        program.append(tmp);
        program.append("\n");
    }
    f.close();

    return program;
}
