#include "Analyze.hpp"

#include <fmt/format.h>

#include <iostream>
#include <string>
#include <vector>

#include "AST.hpp"
#include "Lexer.hpp"
#include "List.hpp"
#include "PascalRules.hpp"
#include "SyntExp.hpp"
#include "Syntax.hpp"
#include "Visitor.hpp"

/// @brief Функции анализа программ на языке Pascal
namespace Analyze {

void highlight(std::string text, List<Token> tokens) {
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
    while (i < text.size()) {
        if (tokenI < tokens.size() - 1 && i == tokens[tokenI].pos()) {
            std::cout << "\033[0m" << colors[tokens[tokenI].getAdvType()];
            std::size_t len = tokens[tokenI].len();
            while (len != 0) {
                std::cout << text[i];
                i++;
                len--;
            }
            tokenI++;
        } else {
            std::cout << "\033[0m\033[30;1m";
            if (tokenI < tokens.size() - 1) {
                while (i != tokens[tokenI].pos()) {
                    std::cout << text[i];
                    i++;
                }
            } else {
                while (i < text.size()) {
                    std::cout << text[i];
                    i++;
                }
            }
        }
    }
    std::cout << "\033[0m\n";
}

void analyzeFile(std::string inName, std::string outName) {
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> tokens = lexer.analyzeFile(inName);

    SyntaxAnalyzer syntax(tokens);
    std::unique_ptr<IAST> root(syntax.analyzeTokens());

    highlight(lexer.getText(), syntax.getTokens());

    GraphvizVisitor graph("out.dot");
    root->accept(graph);
    graph.write();

    int statusGraphviz = system("dot -V >nul 2>nul");
    if (WEXITSTATUS(statusGraphviz) == 0) {
        system(fmt::format("dot out.dot -Tsvg > {}", outName).c_str());
    } else {
        std::cout << "Проблема при определении Graphviz. .dot описание "
                     "изображения сгенерировано, но генерация самого "
                     "изображения требует Graphviz";
    }

    SemanticVisitor semantic;
    semantic.prebuildFunctions(PascalRules::getStandartFuncs());
    root->accept(semantic);
}

void analyzeFile(std::string inName) { analyzeFile(inName, "output.svg"); }

}  // namespace Analyze