#ifndef __INC_PASCALTOKENS_H
#define __INC_PASCALTOKENS_H

/**
 * @file PascalRules.hpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Правила языка Паскаль
 * @version 1.0
 */

#include <string>
#include <vector>

#include "List.hpp"
#include "Token.hpp"
#include "Visitor.hpp"

/// @brief Специальные правила Паскаля
namespace PascalRules {

List<SemanticVisitor::FunctionData> getStandartFuncs(void) {
    List<SemanticVisitor::FunctionData> functions = {
        {{"writeln", IToken::ID, IToken::FUNCTION_NAME},
         std::vector<std::string>{"string"},
         "void"},
        {{"sin", IToken::ID, IToken::FUNCTION_NAME},
         std::vector<std::string>{"real"},
         "real"}};
    return functions;
}

/// @brief Возвращает шаблоны токинов языка Pascal
List<TokenTemplate> getPascalTemplates(void) {
    List<TokenTemplate> templates = {
        //{"^[\t ][^a-zA-Z\\d\\s:]", IToken::ERROR},

        {"^[\t ]*\\+", IToken::Type::PLUS, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\-", IToken::Type::MINUS, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\*", IToken::Type::MUL, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\/", IToken::Type::FLOAT_DIV, IToken::AdvType::OPERATOR},
        {"^[\t ]*div", IToken::Type::INTEGER_DIV, IToken::AdvType::OPERATOR},
        {"^[\t ]*mod", IToken::Type::MOD, IToken::AdvType::OPERATOR},

        {"^[\t ]*\\(", IToken::Type::LPAREN, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\)", IToken::Type::RPAREN, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\{", IToken::Type::LBRACE, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\}", IToken::Type::RBRACE, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\[", IToken::Type::LSQBRACKET, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\]", IToken::Type::RSQBRACKET, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\.\\.", IToken::Type::RANGE, IToken::AdvType::OPERATOR},

        {"^[\t ]*begin", IToken::Type::BEGIN, IToken::AdvType::KEYWORD},
        {"^[\t ]*end", IToken::Type::END, IToken::AdvType::KEYWORD},
        {"^[\t ]*program", IToken::Type::PROGRAM, IToken::AdvType::KEYWORD},
        {"^[\t ]*var", IToken::Type::VAR, IToken::AdvType::KEYWORD},
        {"^[\t ]*const", IToken::Type::CONST, IToken::AdvType::KEYWORD},
        {"^[\t ]*while", IToken::Type::WHILE, IToken::AdvType::KEYWORD},
        {"^[\t ]*for", IToken::Type::FOR, IToken::AdvType::KEYWORD},
        {"^[\t ]*down", IToken::Type::DOWN, IToken::AdvType::KEYWORD},
        {"^[\t ]*down to", IToken::Type::DOWNTO, IToken::AdvType::KEYWORD},
        {"^[\t ]*to", IToken::Type::TO, IToken::AdvType::KEYWORD},
        {"^[\t ]*do", IToken::Type::DO, IToken::AdvType::KEYWORD},
        {"^[\t ]*if", IToken::Type::IF, IToken::AdvType::KEYWORD},
        {"^[\t ]*then", IToken::Type::THEN, IToken::AdvType::KEYWORD},
        {"^[\t ]*else", IToken::Type::ELSE, IToken::AdvType::KEYWORD},
        {"^[\t ]*function", IToken::Type::FUNCTION, IToken::AdvType::KEYWORD},
        {"^[\t ]*procedure", IToken::Type::PROCEDURE, IToken::AdvType::KEYWORD},
        {"^[\t ]*return", IToken::Type::RETURN, IToken::AdvType::KEYWORD},
        {"^[\t ]*array", IToken::Type::ARRAY, IToken::AdvType::KEYWORD},
        {"^[\t ]*of", IToken::Type::OF, IToken::AdvType::KEYWORD},

        {"^[\t ]*integer", IToken::Type::INTEGER, IToken::AdvType::TYPE_SPEC},
        {"^[\t ]*real", IToken::Type::REAL, IToken::AdvType::TYPE_SPEC},
        {"^[\t ]*string", IToken::Type::STRING, IToken::AdvType::TYPE_SPEC},
        {"^[\t ]*void", IToken::Type::VOID, IToken::AdvType::TYPE_SPEC},

        {"^[\t ]*not", IToken::Type::NOT, IToken::AdvType::OPERATOR},
        {"^[\t ]*and", IToken::Type::AND, IToken::AdvType::OPERATOR},
        {"^[\t ]*or", IToken::Type::OR, IToken::AdvType::OPERATOR},

        {"^[\t ]*\\'", IToken::Type::STRING_BOUND, IToken::AdvType::OPERATOR},
        {"^[\t ]*,", IToken::Type::COMMA, IToken::AdvType::OPERATOR},
        {"^[\t ]*:", IToken::Type::COLON, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\.", IToken::Type::DOT, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\;", IToken::Type::SEMI, IToken::AdvType::OPERATOR},

        {"^[\t ]*\\=", IToken::Type::EQ, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\<>", IToken::Type::NEQ, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\<", IToken::Type::LESS, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\<=", IToken::Type::LESS_EQ, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\>", IToken::Type::MORE, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\>=", IToken::Type::MORE_EQ, IToken::AdvType::OPERATOR},

        {"^[\t ]*\\:\\=", IToken::Type::ASSIGN, IToken::AdvType::OPERATOR},

        {"^[\t ]*[0-9]+", IToken::Type::INTEGER_CONST,
         IToken::AdvType::SOME_CONST},
        {"^[\t ]*[0-9]+\\.[0-9]*", IToken::Type::REAL_CONST,
         IToken::AdvType::SOME_CONST},
        {"^[\t ]*'.*'", IToken::STRING_CONST, IToken::AdvType::SOME_CONST},
        {"^[\t ]*\\{(.|\n)*\\}", IToken::COMMENT, IToken::AdvType::NOTPROCESS},

        {"^[\t ]*[_A-Za-z|А-Яа-я]+[[:alnum:]]*\\b", IToken::Type::ID,
         IToken::AdvType::UNKNOWN},
        {"^[\t ]*\n", IToken::NEWLINE, IToken::AdvType::NOTPROCESS}};
    return templates;
}

}  // namespace PascalRules

#endif