#include "SyntExp.hpp"

#include <fmt/format.h>

#include <stdexcept>
#include <string>

#include "Token.hpp"
#include "magic_enum.hpp"

AnalyzeException::AnalyzeException(Token errorTok,
                                   std::string msg = "Analyze Error!")
    : errorTok(errorTok), msg(msg) {
    this->msg = msg + errorTok.getInfo() + "\n";
};


const char *AnalyzeException::what() const throw() { return msg.c_str(); }

LexerException::LexerException(Token errorTok,
                               std::string msg = "Analyze Error!")
    : AnalyzeException(errorTok, msg){};

SyntaxException::SyntaxException(Token errorTok,
                                 std::string msg = "Analyze Error!")
    : AnalyzeException(errorTok, msg){};

SemanticException::SemanticException(Token errorTok,
                                     std::string msg = "Analyze Error!")
    : AnalyzeException(errorTok, msg){};

TypeException::TypeException(Token errorTok, std::string msg = "Analyze Error!")
    : SemanticException(errorTok, msg){};