#include "SyntExp.hpp"

#include <string>
#include <stdexcept>
#include <fmt/format.h>

#include "magic_enum.hpp"

#include "Token.hpp"

AnalyzeException::AnalyzeException(Token errorTok, std::string msg = "Analyze Error!") : errorTok(errorTok), msg(msg){
    this->msg = msg + errorTok.getInfo() + "\n";
};

AnalyzeException::~AnalyzeException(){};

const char *AnalyzeException::what() const throw(){
    return msg.c_str();
}

LexerException::LexerException(Token errorTok, std::string msg = "Analyze Error!") : AnalyzeException(errorTok, msg){};
LexerException::~LexerException(){};

SyntaxException::SyntaxException(Token errorTok, std::string msg = "Analyze Error!") : AnalyzeException(errorTok, msg){};
SyntaxException::~SyntaxException(){};

SemanticException::SemanticException(Token errorTok, std::string msg = "Analyze Error!") : AnalyzeException(errorTok, msg){};
SemanticException::~SemanticException(){};