#ifndef __INC_LEXER_TEST_H
#define __INC_LEXER_TEST_H
#include "List.cpp"
#include "Lexer.hpp"

namespace LexerTestRules{
    /// @brief Возвращает шаблоны токинов языка Pascal
    List<TokenTemplate> getMathRules(void){
         List<TokenTemplate> templates = {
                std::vector<TokenTemplate>{

                {"^\\s*\\+", IToken::Type::PLUS},
                {"^\\s*\\-", IToken::Type::MINUS},
                {"^\\s*\\*", IToken::Type::MUL},
                {"^\\s*\\/", IToken::Type::FLOAT_DIV},

                {"^\\s*\\(", IToken::Type::LPAREN},
                {"^\\s*\\)", IToken::Type::RPAREN},

                {"^\\s*\\=", IToken::Type::EQ},

                {"^\\s*[0-9]+", IToken::Type::INTEGER_CONST},
                {"^\\s*[0-9]+\\.[0-9]*", IToken::Type::REAL_CONST},

            }
        };
        return templates;
    }


}

#endif