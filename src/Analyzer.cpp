#include "Analyzer.hpp"

#include <fmt/format.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "ASTclasses.hpp"
#include "Lexer.hpp"
#include "List.hpp"
#include "Syntax.hpp"

Analyzer::Analyzer(std::unique_ptr<ISyntaxAnalyzer> syntax,
                   List<TokenTemplate> templates)
    : syntax(std::move(syntax)) {
    lexer.setTemplates(templates);
};

void Analyzer::addVisitor(std::unique_ptr<IVisitor> visitor) {
    visitorsToRun.emplace_back(std::move(visitor));
}

void Analyzer::highlightOutput() {
    std::string text = lexer.getText();
    List<Token> tokens = syntax->getTokens();
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

void Analyzer::analyzeFile(std::string inName) {
    syntax->setTokens(lexer.analyzeFile(inName));

    std::unique_ptr<IAST> root(syntax->analyzeTokens());

    for (std::size_t i = 0; i < visitorsToRun.size(); i++) {
        root->accept(*visitorsToRun[i]);
    }
}
