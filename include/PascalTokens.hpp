#ifndef __INC_PASCALTOKENS_H
#define __INC_PASCALTOKENS_H

#include <vector>

#include "Token.hpp"
#include "List.cpp"

/// @brief Специальные правила Паскаля
namespace PascalRules{

//    Token CONST(std::size_t lineNum){
//        return {"CONST", IToken::CONST, lineNum};
//    }
//
//    Token VAR(std::size_t lineNum){
//        return {"VAR", IToken::VAR, lineNum};
//    }

    /// @brief Возвращает шаблоны токинов языка Pascal
    List<TokenTemplate> getPascalTemplates(void){
         List<TokenTemplate> templates = {
        //{"^[\t ][^a-zA-Z\\d\\s:]", IToken::ERROR},

        {"^[\t ]*\\+", IToken::Type::PLUS, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\-", IToken::Type::MINUS, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\*", IToken::Type::MUL, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\/", IToken::Type::FLOAT_DIV, IToken::AdvType::OPERATOR},
        {"^[\t ]*DIV", IToken::Type::INTEGER_DIV, IToken::AdvType::OPERATOR},
        {"^[\t ]*MOD", IToken::Type::MOD, IToken::AdvType::OPERATOR},

        {"^[\t ]*\\(", IToken::Type::LPAREN, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\)", IToken::Type::RPAREN, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\{", IToken::Type::LBRACE, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\}", IToken::Type::RBRACE, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\[", IToken::Type::LSQBRACKET, IToken::AdvType::OPERATOR},
        {"^[\t ]*\\]", IToken::Type::RSQBRACKET, IToken::AdvType::OPERATOR},


        {"^[\t ]*BEGIN", IToken::Type::BEGIN, IToken::AdvType::KEYWORD},
        {"^[\t ]*END", IToken::Type::END, IToken::AdvType::KEYWORD},
        {"^[\t ]*PROGRAM", IToken::Type::PROGRAM, IToken::AdvType::KEYWORD},
        {"^[\t ]*VAR", IToken::Type::VAR, IToken::AdvType::KEYWORD},
        {"^[\t ]*INTEGER", IToken::Type::INTEGER, IToken::AdvType::TYPE_SPEC},
        {"^[\t ]*REAL", IToken::Type::REAL, IToken::AdvType::TYPE_SPEC},
        {"^[\t ]*CONST", IToken::Type::CONST, IToken::AdvType::KEYWORD},
        {"^[\t ]*WHILE", IToken::Type::WHILE, IToken::AdvType::KEYWORD},
        {"^[\t ]*DO", IToken::Type::DO, IToken::AdvType::KEYWORD},
        {"^[\t ]*IF", IToken::Type::IF, IToken::AdvType::KEYWORD},
        {"^[\t ]*THEN", IToken::Type::THEN, IToken::AdvType::KEYWORD},
        {"^[\t ]*ELSE", IToken::Type::ELSE, IToken::AdvType::KEYWORD},

        {"^[\t ]*NOT", IToken::Type::NOT, IToken::AdvType::OPERATOR},
        {"^[\t ]*AND", IToken::Type::AND, IToken::AdvType::OPERATOR},
        {"^[\t ]*OR", IToken::Type::OR, IToken::AdvType::OPERATOR},

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

        {"^[\t ]*[0-9]+", IToken::Type::INTEGER_CONST, IToken::AdvType::SOME_CONST},
        {"^[\t ]*[0-9]+\\.[0-9]*", IToken::Type::REAL_CONST, IToken::AdvType::SOME_CONST},
        {"^[\t ]*'.*'", IToken::STRING_CONST, IToken::AdvType::SOME_CONST},
        {"^[\t ]*\\{(.|\n)*\\}", IToken::COMMENT, IToken::AdvType::NOTPROCESS},

        {"^[\t ]*[_A-Za-z|А-Яа-я]+[[:alnum:]]*\\b", IToken::Type::ID, IToken::AdvType::UNKNOWN},
        {"^[\t ]*\n", IToken::NEWLINE, IToken::AdvType::NOTPROCESS}
        };
        return templates;
    }

}

#endif