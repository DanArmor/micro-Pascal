#ifndef _INC_SYNTAX_H
#define _INC_SYNTAX_H

/**
 * @file Syntax.hpp
 * @author DanArmor
 * @brief Интерфейс синтаксического анализатора
 * @version 1.0
 */

#include <memory>

#include "ASTclasses.hpp"
#include "List.hpp"
#include "Token.hpp"

/// @brief Интерфейс синтаксических анализаторов
class ISyntaxAnalyzer {
   public:
    virtual std::unique_ptr<IAST> analyzeTokens(void) = 0;
    virtual void setTokens(List<Token> const &newTokens) = 0;
    virtual List<Token> getTokens(void) = 0;
};

#endif