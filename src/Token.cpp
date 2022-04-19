#include "Token.hpp"
#include "magic_enum.hpp"
#include <fmt/format.h>

/*Определения IToken
==================*/
IToken::IToken(void){};

IToken::IToken(Type type) : type(type){};

IToken::IToken(Type type, AdvType advType) : type(type), advType(advType){};


/*Определения Token
==================*/
Token::Token(){};

Token::Token(std::string str, Type type) : IToken(type), str(str){};

Token::Token(std::string str, Type type, std::size_t lineNum) : IToken(type), str(str), lineNum(lineNum){};

Token::Token(std::string str, Type type, std::size_t lineNum, std::size_t inLinePosNum, std::size_t inFilePos) : IToken(type), str(str), lineNum(lineNum), posInLineNum(inLinePosNum), posNum(inFilePos){};

Token::Token(std::string str, Type type, AdvType advType, std::size_t lineNum, std::size_t inLinePosNum, std::size_t inFilePos) : IToken(type, advType), str(str), lineNum(lineNum), posInLineNum(inLinePosNum), posNum(inFilePos){};

std::string const & Token::getStr(void){
    return str;
}

std::size_t Token::line(void){
    return lineNum+1;
}

std::size_t Token::rawLine(void){
    return lineNum;
}

std::size_t Token::posInLine(void){
    return posInLineNum + 1;
}

std::size_t Token::rawPosInLine(void){
    return posInLineNum;
}

std::size_t Token::pos(void){
    return posNum;
}

std::size_t Token::len(void){
    return getStr().size();
}

std::string Token::getInfo(void){
    return fmt::format("::TOKEN_INFO::\nTYPE: {}  \nSTR: {} \nLINE: {}", magic_enum::enum_name(getType()), getStr(), line());
}

/* Определения TokenTemplate
==================*/

TokenTemplate::TokenTemplate(){};

TokenTemplate::TokenTemplate(std::string str, Type type, AdvType advType) : IToken(type, advType), regStr(str), regTemplate(str){};

std::regex const & TokenTemplate::getRegex(void){
    return regTemplate;
}