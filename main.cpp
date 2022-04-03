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

class Variable{
    public:

    private:
    std::variant<int, double, char, bool> value;
};

int main(void){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> tokens = lexer.analyzeFile("prog1.txt");

//    for(auto tok : tokens){
//        std::cout << fmt::format("{}     {}\n", tok.getStr(), magic_enum::enum_name(tok.getType()));
//    }

    SyntaxAnalyzer syntax(tokens);
    AST *root = syntax.parseTokens();

    GraphvizVisitor graph("out.dot");
    root->accept(graph);
    graph.write();

//    CalcVisitor calc;
//    root->accept(calc);
//
//    for(auto p : globalVariables)
//        std::cout << fmt::format("{}={}\n", p.first, p.second);
    return 0;
}
