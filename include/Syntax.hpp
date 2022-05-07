#ifndef _INC_SYNTAX_H
#define _INC_SYNTAX_H

#include <memory>

#include "List.hpp"
#include "ASTclasses.hpp"
#include "Token.hpp"

class SyntaxAnalyzer {
    public:
    virtual std::unique_ptr<IAST> analyzeTokens(void) = 0;
    virtual void setTokens(List<Token> const &newTokens) = 0;
    virtual List<Token> getTokens(void) = 0;
};

#endif