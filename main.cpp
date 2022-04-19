#include <map>
#include <string>
#include <iostream>
#include <variant>
#include <any>

std::map<std::string, int> globalVariables;

#include "List.cpp"

#include "Token.hpp"
#include "Lexer.hpp"
#include "Syntax.hpp"

#include "PascalTokens.hpp"

int main(int argc, char** argv){
    if(argc != 2){
        std::cout << "Пожалуйста, введите только\033[31m название исходного файла\33[0m для разбора после названия программы\n";
        exit(1);
    }
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> tokens = lexer.analyzeFile(argv[1]);

//    for(auto tok : tokens){
//        std::cout << fmt::format("{}     {}\n", tok.getStr(), magic_enum::enum_name(tok.getType()));
//    }
    //List<Token>  tokens{{Token("PROGRAM", IToken::PROGRAM), Token("NAME", IToken::ID), Token(";", IToken::SEMI), Token("BEGIN", IToken::BEGIN), Token("END", IToken::END), Token(".", IToken::DOT), Token("$", IToken::ENDOFSTREAM)}};
    SyntaxAnalyzer syntax(tokens);
    std::unique_ptr<AST>root(syntax.parseTokens());

    GraphvizVisitor graph("out.dot");
    root->accept(graph);
    graph.write();

//    CodeGenVisitor code("test.s");
//    root->accept(code);
//    code.done();

//    CalcVisitor calc;
//    root->accept(calc);
//
//    for(auto p : globalVariables)
//        std::cout << fmt::format("{}={}\n", p.first, p.second);
    return 0;
}
