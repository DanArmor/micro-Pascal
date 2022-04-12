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
                std::vector<TokenTemplate>{
      //{"^[\t ][^a-zA-Z\\d\\s:]", IToken::ERROR},

      {"^[\t ]*\\+", IToken::Type::PLUS},
      {"^[\t ]*\\-", IToken::Type::MINUS},
      {"^[\t ]*\\*", IToken::Type::MUL},
      {"^[\t ]*\\/", IToken::Type::FLOAT_DIV},
      {"^[\t ]*DIV", IToken::Type::INTEGER_DIV},
      {"^[\t ]*MOD", IToken::Type::MOD},

      {"^[\t ]*\\(", IToken::Type::LPAREN},
      {"^[\t ]*\\)", IToken::Type::RPAREN},
      {"^[\t ]*\\{", IToken::Type::LBRACE},
      {"^[\t ]*\\}", IToken::Type::RBRACE},
      {"^[\t ]*\\[", IToken::Type::LSQBRACKET},
      {"^[\t ]*\\]", IToken::Type::RSQBRACKET},


      {"^[\t ]*BEGIN", IToken::Type::BEGIN},
      {"^[\t ]*END", IToken::Type::END},
      {"^[\t ]*PROGRAM", IToken::Type::PROGRAM},
      {"^[\t ]*VAR", IToken::Type::VAR},
      {"^[\t ]*INTEGER", IToken::Type::INTEGER},
      {"^[\t ]*REAL", IToken::Type::REAL},
      {"^[\t ]*CONST", IToken::Type::CONST},
      {"^[\t ]*WHILE", IToken::Type::WHILE},
      {"^[\t ]*DO", IToken::Type::DO},
      {"^[\t ]*IF", IToken::Type::IF},
      {"^[\t ]*THEN", IToken::Type::THEN},
      {"^[\t ]*ELSE", IToken::Type::ELSE},

      {"^[\t ]*NOT", IToken::Type::NOT},
      {"^[\t ]*AND", IToken::Type::AND},
      {"^[\t ]*OR", IToken::Type::OR},

      {"^[\t ]*\\'", IToken::Type::STRING_BOUND},
      {"^[\t ]*,", IToken::Type::COMMA},
      {"^[\t ]*:", IToken::Type::COLON},
      {"^[\t ]*\\.", IToken::Type::DOT},
      {"^[\t ]*\\;", IToken::Type::SEMI},

      {"^[\t ]*\\=", IToken::Type::EQ},
      {"^[\t ]*\\<>", IToken::Type::NEQ},
      {"^[\t ]*\\<", IToken::Type::LESS},
      {"^[\t ]*\\<=", IToken::Type::LESS_EQ},
      {"^[\t ]*\\>", IToken::Type::MORE},
      {"^[\t ]*\\>=", IToken::Type::MORE_EQ},

      {"^[\t ]*\\:\\=", IToken::Type::ASSIGN},

      {"^[\t ]*[0-9]+", IToken::Type::INTEGER_CONST},
      {"^[\t ]*[0-9]+\\.[0-9]*", IToken::Type::REAL_CONST},
      {"^[\t ]*'.*'", IToken::STRING_CONST},
      {"^[\t ]*\\{(.|\n)*\\}", IToken::COMMENT},

      {"^[\t ]*[_A-Za-z|А-Яа-я]+[[:alnum:]]*\\b", IToken::Type::ID},
      {"^\\s*\n", IToken::NEWLINE}

    }
        };
        return templates;
    }

}

#endif