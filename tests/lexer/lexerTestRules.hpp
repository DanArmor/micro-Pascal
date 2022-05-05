#ifndef __INC_LEXER_TEST_H
#define __INC_LEXER_TEST_H

/**
 * @file lexerTestRules.hpp
 * @author DanArmor
 * @brief Тестовые правила для Лексера
 * @version 0.1
 * @copyright Copyright (c) 2022
 */

#include "List.hpp"
#include "Lexer.hpp"

/// @brief Тестовые правила для лексера
namespace LexerTestRules{
    /// @brief Возвращает шаблоны токинов языка Pascal
    List<TokenTemplate> getMathRules(void){
         List<TokenTemplate> templates = {
                {"^\\s*\\+", IToken::Type::PLUS, IToken::AdvType::OPERATOR},
                {"^\\s*\\-", IToken::Type::MINUS, IToken::AdvType::OPERATOR},
                {"^\\s*\\*", IToken::Type::MUL, IToken::AdvType::OPERATOR},
                {"^\\s*\\/", IToken::Type::FLOAT_DIV, IToken::AdvType::OPERATOR},

                {"^\\s*\\(", IToken::Type::LPAREN, IToken::AdvType::OPERATOR},
                {"^\\s*\\)", IToken::Type::RPAREN, IToken::AdvType::OPERATOR},

                {"^\\s*\\=", IToken::Type::EQ, IToken::AdvType::OPERATOR},

                {"^\\s*[0-9]+", IToken::Type::INTEGER_CONST, IToken::AdvType::SOME_CONST},
                {"^\\s*[0-9]+\\.[0-9]*", IToken::Type::REAL_CONST, IToken::AdvType::SOME_CONST},
        };
        return templates;
    }


}

#endif