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

void highlight(std::string text, List<Token> tokens){
    std::map<IToken::AdvType, std::string> colors = {
        {IToken::AdvType::SOME_CONST, "\033[31m"},
        {IToken::AdvType::FUNCTION_NAME, "\033[32m"},
        {IToken::AdvType::VAR_NAME, "\033[33m"},
        {IToken::AdvType::TYPE_SPEC, "\033[1;34m"},
        {IToken::AdvType::KEYWORD, "\033[35m"},
        {IToken::AdvType::OPERATOR, "\033[36m"},
        {IToken::AdvType::PROGRAM_NAME, "\033[1;31m"},
        {IToken::AdvType::UNKNOWN, "\033[4;37m"},
        {IToken::AdvType::NOTPROCESS, "\033[37m"},
    };
    int tokenI = 0;
    int i = 0;
    while(i < text.size()){
        if(i == tokens[tokenI].pos()){
            std::cout << "\033[0m" << colors[tokens[tokenI].getAdvType()];
            std::size_t len = tokens[tokenI].len();
            while(len != 0){
                std::cout << text[i];
                i++;
                len--;
            }
            tokenI++;
        } else{
            std::cout << "\033[0m\033[30;1m";
            while(i != tokens[tokenI].pos()){
                std::cout << text[i];
                i++;
            }
        }
    }
    std::cout << "\033[0m\n";
}

int main(int argc, char** argv){

    if(argc != 2){
        std::cout << "Пожалуйста, введите только\033[31m название исходного файла\33[0m для разбора после названия программы\n";
        exit(1);
    }

    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> tokens = lexer.analyzeFile(argv[1]);


//    for(auto tok : tokens){
//        std::cout << fmt::format("Str:{} Type:{}\nLine:{} InLinePos:{} Absolute:{}\n\n", tok.getStr(), magic_enum::enum_name(tok.getType()), std::to_string(tok.line()), std::to_string(tok.posInLine()), std::to_string(tok.pos()));
//    }

//    for(auto tok : tokens){
//        std::cout << fmt::format("{}     {}\n", tok.getStr(), magic_enum::enum_name(tok.getType()));
//    }
    //List<Token>  tokens{{Token("PROGRAM", IToken::PROGRAM), Token("NAME", IToken::ID), Token(";", IToken::SEMI), Token("BEGIN", IToken::BEGIN), Token("END", IToken::END), Token(".", IToken::DOT), Token("$", IToken::ENDOFSTREAM)}};
    
    
    SyntaxAnalyzer syntax(tokens);
    std::unique_ptr<AST>root(syntax.parseTokens());

    HighlightAccurateVisitor hl(tokens);
    root->accept(hl);

    highlight(lexer.getText(), hl.getTokens());

    for(auto tt : hl.getTokens()){
        std::cout << tt.getInfo() << "\n";
    }


//
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
