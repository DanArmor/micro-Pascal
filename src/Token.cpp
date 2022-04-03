#include "Token.hpp"
#include "magic_enum.hpp"
#include <fmt/format.h>

/*Определения IToken
==================*/
IToken::IToken(void){};

IToken::IToken(Type type) : type(type){};


/*Определения Token
==================*/
Token::Token(){};

Token::Token(std::string str, Type type) : IToken(type), str(str){};

Token::Token(std::string str, Type type, std::size_t lineNum) : IToken(type), str(str), lineNum(lineNum){};

std::string const & Token::getStr(void){
    return str;
}

std::size_t Token::line(void){
    return lineNum+1;
}

std::size_t Token::rowLine(void){
    return lineNum;
}

std::string Token::getInfo(void){
    return fmt::format("::TOKEN_INFO::\nTYPE: {}  \nSTR: {} \nLINE: {}", magic_enum::enum_name(getType()), getStr(), line());
}

/* Определения TokenTemplate
==================*/

TokenTemplate::TokenTemplate(){};

TokenTemplate::TokenTemplate(std::string str, Type type) : IToken(type), regStr(str), regTemplate(str){};

std::regex const & TokenTemplate::getRegex(void){
    return regTemplate;
}