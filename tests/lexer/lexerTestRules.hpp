#ifndef __INC_LEXER_TEST_H
#define __INC_LEXER_TEST_H
#include "List.cpp"
#include "Lexer.hpp"

namespace LexerTestRules{
    /// @brief Возвращает шаблоны токинов языка Pascal
    List<TokenTemplate> getMathRules(void){
         List<TokenTemplate> templates = {
                std::vector<TokenTemplate>{

                {"^\\+", IToken::Type::PLUS},
                {"^\\-", IToken::Type::MINUS},
                {"^\\*", IToken::Type::MUL},
                {"^\\/", IToken::Type::FLOAT_DIV},

                {"^\\(", IToken::Type::LPAREN},
                {"^\\)", IToken::Type::RPAREN},

                {"^\\=", IToken::Type::EQ},

                {"^[0-9]+", IToken::Type::INTEGER_CONST},
                {"^[0-9]+\\.[0-9]*", IToken::Type::REAL_CONST},

            }
        };
        return templates;
    }

    /// @brief Возвращает шаблоны токинов языка Pascal
    List<TokenTemplate> getPascalTemplates(void){
         List<TokenTemplate> templates = {
                std::vector<TokenTemplate>{

                {"^\\+", IToken::Type::PLUS},
                {"^\\-", IToken::Type::MINUS},
                {"^\\*", IToken::Type::MUL},
                {"^\\/", IToken::Type::FLOAT_DIV},
                {"^DIV", IToken::Type::INTEGER_DIV},
                {"^MOD", IToken::Type::MOD},

                {"^\\(", IToken::Type::LPAREN},
                {"^\\)", IToken::Type::RPAREN},
                {"^\\{", IToken::Type::LBRACE},
                {"^\\}", IToken::Type::RBRACE},
                {"^\\[", IToken::Type::LSQBRACKET},
                {"^\\]", IToken::Type::RSQBRACKET},

                {"^\\:\\=", IToken::Type::ASSIGN},

                {"^BEGIN", IToken::Type::BEGIN},
                {"^END", IToken::Type::END},
                {"^PROGRAM", IToken::Type::PROGRAM},
                {"^VAR", IToken::Type::VAR},
                {"^INTEGER", IToken::Type::INTEGER},
                {"^REAL", IToken::Type::REAL},
                {"^CONST", IToken::Type::CONST},
                {"^WHILE", IToken::Type::WHILE},
                {"^DO", IToken::Type::DO},
                {"^IF", IToken::Type::IF},
                {"^THEN", IToken::Type::THEN},
                {"^ELSE", IToken::Type::ELSE},

                {"^NOT", IToken::Type::NOT},
                {"^AND", IToken::Type::AND},
                {"^OR", IToken::Type::OR},

                {"^\\'", IToken::Type::STRING_BOUND},
                {"^,", IToken::Type::COMMA},
                {"^:", IToken::Type::COLON},
                {"^\\.", IToken::Type::DOT},
                {"^\\;", IToken::Type::SEMI},

                {"^\\=", IToken::Type::EQ},
                {"^\\<>", IToken::Type::NEQ},
                {"^\\<", IToken::Type::LESS},
                {"^\\<=", IToken::Type::LESS_EQ},
                {"^\\>", IToken::Type::MORE},
                {"^\\>=", IToken::Type::MORE_EQ},

                {"^[0-9]+", IToken::Type::INTEGER_CONST},
                {"^[0-9]+\\.[0-9]*", IToken::Type::REAL_CONST},

                {"^\\b[_A-Za-z | А-Яа-я]+[[:alnum:]]*\\b", IToken::Type::ID}
            }
        };
        return templates;
    }

}

#endif