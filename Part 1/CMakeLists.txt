cmake_minimum_required(VERSION 3.19)
project(CPS2000-Assignment)

set(CMAKE_CXX_STANDARD 14)


set(AST AST/AST.cpp)
set(Lexer Lexer/Lexer.cpp)
set(Parser Parser/Parser.cpp)
set(Symbol SymbolTable/SymbolTable.cpp)
set(Token Token/Token.cpp)
set(Visitors Visitor/XMLVisitor.cpp Visitor/SemanticVisitor.cpp Visitor/IntepreterVisitor.cpp)


add_executable(TeaLang main.cpp ${AST} ${Lexer} ${Parser} ${Symbol} ${Token} ${Visitors})