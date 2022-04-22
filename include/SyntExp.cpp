#ifndef __INC_SYNTEXP_H
#define __INC_SYNTEXP_H

#include <string>
#include <stdexcept>
#include <fmt/format.h>

#include "magic_enum.hpp"

#include "Token.hpp"
#include <iostream>
class AnalyzeException : public std::exception{
    public:

    AnalyzeException(Token errorTok, std::string msg = "Analyze Error!") : errorTok(errorTok), msg(msg){
        this->msg = msg + errorTok.getInfo() + "\n";
    };
    ~AnalyzeException(){};

    const char *what() const throw(){
        return msg.c_str();
    }

    protected:
    std::string msg;
    Token errorTok;
};

class LexerException : public AnalyzeException{
    public:
    LexerException(Token errorTok, std::string msg = "Analyze Error!") : AnalyzeException(errorTok, msg){};
    ~LexerException(){};
};

class SyntaxException : public AnalyzeException{
    public:
    SyntaxException(Token errorTok, std::string msg = "Analyze Error!") : AnalyzeException(errorTok, msg){};
    ~SyntaxException(){};
};

class SemanticException : public AnalyzeException{
    public:
    SemanticException(Token errorTok, std::string msg = "Analyze Error!") : AnalyzeException(errorTok, msg){};
    ~SemanticException(){};
};

#endif