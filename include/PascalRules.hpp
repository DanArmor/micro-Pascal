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
        {{"writeln", BaseToken::ID, BaseToken::FUNCTION_NAME},
         std::vector<std::string>{"string"},
         "void"},
        {{"sin", BaseToken::ID, BaseToken::FUNCTION_NAME},
         std::vector<std::string>{"real"},
         "real"}};
    return functions;
}

/// @brief Возвращает шаблоны токинов языка Pascal
List<TokenTemplate> getPascalTemplates(void) {
    List<TokenTemplate> templates = {
        //{"^[\t ][^a-zA-Z\\d\\s:]", BaseToken::ERROR},

        {"^[\t ]*\\+", BaseToken::Type::PLUS, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*\\-", BaseToken::Type::MINUS, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*\\*", BaseToken::Type::MUL, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*\\/", BaseToken::Type::FLOAT_DIV, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*div", BaseToken::Type::INTEGER_DIV, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*mod", BaseToken::Type::MOD, BaseToken::AdvType::OPERATOR},

        {"^[\t ]*\\(", BaseToken::Type::LPAREN, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*\\)", BaseToken::Type::RPAREN, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*\\{", BaseToken::Type::LBRACE, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*\\}", BaseToken::Type::RBRACE, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*\\[", BaseToken::Type::LSQBRACKET, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*\\]", BaseToken::Type::RSQBRACKET, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*\\.\\.", BaseToken::Type::RANGE, BaseToken::AdvType::OPERATOR},

        {"^[\t ]*begin", BaseToken::Type::BEGIN, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*end", BaseToken::Type::END, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*program", BaseToken::Type::PROGRAM, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*var", BaseToken::Type::VAR, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*const", BaseToken::Type::CONST, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*while", BaseToken::Type::WHILE, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*for", BaseToken::Type::FOR, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*down", BaseToken::Type::DOWN, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*down to", BaseToken::Type::DOWNTO, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*to", BaseToken::Type::TO, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*do", BaseToken::Type::DO, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*if", BaseToken::Type::IF, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*then", BaseToken::Type::THEN, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*else", BaseToken::Type::ELSE, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*function", BaseToken::Type::FUNCTION, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*procedure", BaseToken::Type::PROCEDURE, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*return", BaseToken::Type::RETURN, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*array", BaseToken::Type::ARRAY, BaseToken::AdvType::KEYWORD},
        {"^[\t ]*of", BaseToken::Type::OF, BaseToken::AdvType::KEYWORD},

        {"^[\t ]*integer", BaseToken::Type::INTEGER, BaseToken::AdvType::TYPE_SPEC},
        {"^[\t ]*real", BaseToken::Type::REAL, BaseToken::AdvType::TYPE_SPEC},
        {"^[\t ]*string", BaseToken::Type::STRING, BaseToken::AdvType::TYPE_SPEC},
        {"^[\t ]*void", BaseToken::Type::VOID, BaseToken::AdvType::TYPE_SPEC},

        {"^[\t ]*not", BaseToken::Type::NOT, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*and", BaseToken::Type::AND, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*or", BaseToken::Type::OR, BaseToken::AdvType::OPERATOR},

        {"^[\t ]*\\'", BaseToken::Type::STRING_BOUND, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*,", BaseToken::Type::COMMA, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*:", BaseToken::Type::COLON, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*\\.", BaseToken::Type::DOT, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*\\;", BaseToken::Type::SEMI, BaseToken::AdvType::OPERATOR},

        {"^[\t ]*\\=", BaseToken::Type::EQ, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*\\<>", BaseToken::Type::NEQ, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*\\<", BaseToken::Type::LESS, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*\\<=", BaseToken::Type::LESS_EQ, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*\\>", BaseToken::Type::MORE, BaseToken::AdvType::OPERATOR},
        {"^[\t ]*\\>=", BaseToken::Type::MORE_EQ, BaseToken::AdvType::OPERATOR},

        {"^[\t ]*\\:\\=", BaseToken::Type::ASSIGN, BaseToken::AdvType::OPERATOR},

        {"^[\t ]*[0-9]+", BaseToken::Type::INTEGER_CONST,
         BaseToken::AdvType::SOME_CONST},
        {"^[\t ]*[0-9]+\\.[0-9]*", BaseToken::Type::REAL_CONST,
         BaseToken::AdvType::SOME_CONST},
        {"^[\t ]*'.*'", BaseToken::STRING_CONST, BaseToken::AdvType::SOME_CONST},
        {"^[\t ]*\\{(.|\n)*\\}", BaseToken::COMMENT, BaseToken::AdvType::NOTPROCESS},

        {"^[\t ]*[_A-Za-z|А-Яа-я]+[[:alnum:]]*\\b", BaseToken::Type::ID,
         BaseToken::AdvType::UNKNOWN},
        {"^[\t ]*\n", BaseToken::NEWLINE, BaseToken::AdvType::NOTPROCESS}};
    return templates;
}

}  // namespace PascalRules

#endif