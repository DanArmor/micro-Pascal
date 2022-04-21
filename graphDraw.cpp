#include <map>
#include <string>
#include <iostream>
#include <variant>
#include <any>

#include "List.cpp"

#include "Token.hpp"
#include "Lexer.hpp"
#include "Syntax.hpp"
#include "Visitor.hpp"

#include "PascalTokens.hpp"

int main(int argc, char** argv){

    if(argc != 2){
        std::cout << "Пожалуйста, введите только\033[31m название исходного файла\33[0m для разбора после названия программы\n";
        exit(1);
    }

    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> tokens = lexer.analyzeFile(argv[1]);

    SyntaxAnalyzer syntax(tokens);
    std::unique_ptr<AST>root(syntax.parseTokens());

    GraphvizVisitor graph("out.dot");
    root->accept(graph);
    graph.write();

    return 0;
}
