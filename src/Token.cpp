#include "Token.hpp"

#include <fmt/format.h>

#include "magic_enum.hpp"


/*Определения IToken
==================*/
IToken::IToken(void){};

IToken::IToken(Type type) : type(type){};

IToken::IToken(Type type, AdvType advType) : type(type), advType(advType){};


/*Определения Token
==================*/
Token::Token() : IToken(IToken::ERROR, IToken::UNKNOWN), str(""){};

Token::Token(IToken::AdvType const type) : IToken(type){};
Token::Token(IToken::Type const type) : IToken(type){};

Token::Token(std::string const &str, Type const type) : IToken(type), str(str){};
Token::Token(std::string const &str, AdvType const type) : str(str), IToken(type){};

Token::Token(std::string const &str, Type const type, AdvType const advType, std::size_t const lineNum, std::size_t const inLinePosNum, std::size_t const inFilePos) : IToken(type, advType), str(str), lineNum(lineNum), posInLineNum(inLinePosNum), posNum(inFilePos){};

std::string const & Token::getStr(void) const{
    return str;
}

void Token::setStr(std::string const &toSet){
    str = toSet;
}

std::size_t Token::line(void) const{
    return lineNum+1;
}

std::size_t Token::rawLine(void) const{
    return lineNum;
}

std::size_t Token::posInLine(void) const{
    return posInLineNum + 1;
}

std::size_t Token::rawPosInLine(void) const{
    return posInLineNum;
}

std::size_t Token::pos(void) const{
    return posNum;
}

std::size_t Token::len(void){
    return getStr().size();
}

std::string Token::getInfo(void) const{
    return fmt::format("Token {0}{2}{1} of type {0}{3}{1} and advType {0}{4}{1} on {0}line {5} : {6}{1}", 
    "\033[31m", "\033[0m", getStr(), magic_enum::enum_name(getType()), magic_enum::enum_name(getAdvType()), line(), posInLine());
}

/* Определения TokenTemplate
==================*/

TokenTemplate::TokenTemplate(){};

TokenTemplate::TokenTemplate(std::string str, Type type, AdvType advType) : IToken(type, advType), regStr(str), regTemplate(str){};

std::regex const & TokenTemplate::getRegex(void){
    return regTemplate;
}