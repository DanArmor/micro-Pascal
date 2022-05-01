#ifndef __INC_SYNTEXP_H
#define __INC_SYNTEXP_H

#include <string>
#include <stdexcept>
#include <fmt/format.h>

#include "magic_enum.hpp"

#include "Token.hpp"

class AnalyzeException : public std::exception{
    public:

    AnalyzeException(Token errorTok, std::string msg);
    ~AnalyzeException();

    const char *what() const throw();

    protected:
    std::string msg;
    Token errorTok;
};

class LexerException : public AnalyzeException{
    public:
    LexerException(Token errorTok, std::string msg);
    ~LexerException();
};

class SyntaxException : public AnalyzeException{
    public:
    SyntaxException(Token errorTok, std::string msg);
    ~SyntaxException();
};

class SemanticException : public AnalyzeException{
    public:
    SemanticException(Token errorTok, std::string msg);
    ~SemanticException();
};

class TypeException : public SemanticException{
    public:
    TypeException(Token errorTok, std::string msg);
    ~TypeException();
};

#endif