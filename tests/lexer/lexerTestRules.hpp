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
                {"^\\s*\\+", BaseToken::Type::PLUS, BaseToken::AdvType::OPERATOR},
                {"^\\s*\\-", BaseToken::Type::MINUS, BaseToken::AdvType::OPERATOR},
                {"^\\s*\\*", BaseToken::Type::MUL, BaseToken::AdvType::OPERATOR},
                {"^\\s*\\/", BaseToken::Type::FLOAT_DIV, BaseToken::AdvType::OPERATOR},

                {"^\\s*\\(", BaseToken::Type::LPAREN, BaseToken::AdvType::OPERATOR},
                {"^\\s*\\)", BaseToken::Type::RPAREN, BaseToken::AdvType::OPERATOR},

                {"^\\s*\\=", BaseToken::Type::EQ, BaseToken::AdvType::OPERATOR},

                {"^\\s*[0-9]+", BaseToken::Type::INTEGER_CONST, BaseToken::AdvType::SOME_CONST},
                {"^\\s*[0-9]+\\.[0-9]*", BaseToken::Type::REAL_CONST, BaseToken::AdvType::SOME_CONST},
        };
        return templates;
    }


}

#endif