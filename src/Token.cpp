#include "Token.hpp"

#include <fmt/format.h>

#include "magic_enum.hpp"

/*Определения IToken
==================*/
BaseToken::BaseToken(void) {}

BaseToken::BaseToken(Type type, AdvType advType) : type(type), advType(advType) {}

BaseToken::Type BaseToken::getType(void) const { return type; }

BaseToken::AdvType BaseToken::getAdvType(void) const { return advType; }

void BaseToken::setAdvType(BaseToken::AdvType newAdvType) { advType = newAdvType; }

/*Определения Token
==================*/
Token::Token() : BaseToken(BaseToken::ERROR, BaseToken::UNKNOWN), str("") {}

Token::Token(std::string const &str, Type const type, AdvType const advType)
    : BaseToken(type, advType), str(str) {}

Token::Token(std::string const &str, Type const type, AdvType const advType,
             std::size_t const lineNum, std::size_t const inLinePosNum,
             std::size_t const inFilePos)
    : BaseToken(type, advType),
      str(str),
      lineNum(lineNum),
      posInLineNum(inLinePosNum),
      posNum(inFilePos) {}

std::string const &Token::getStr(void) const { return str; }

void Token::setStr(std::string const &toSet) { str = toSet; }

std::size_t Token::line(void) const { return lineNum + 1; }

std::size_t Token::rawLine(void) const { return lineNum; }

std::size_t Token::posInLine(void) const { return posInLineNum + 1; }

std::size_t Token::rawPosInLine(void) const { return posInLineNum; }

std::size_t Token::pos(void) const { return posNum; }

std::size_t Token::len(void) { return getStr().size(); }

std::string Token::getInfo(void) const {
    return fmt::format(
        "Токен {0}{2}{1} типа {0}{3}{1} и доп. типа {0}{4}{1} на {0}строке {5} "
        ": {6}{1}. Абсолютная позиция: {0}{7}{1}",
        "\033[31m", "\033[0m", getStr(), magic_enum::enum_name(getType()),
        magic_enum::enum_name(getAdvType()), line(), posInLine(), pos());
}

/* Определения TokenTemplate
==================*/

TokenTemplate::TokenTemplate() {}

TokenTemplate::TokenTemplate(std::string const &str, Type type, AdvType advType)
    : BaseToken(type, advType), regStr(str), regTemplate(str) {}

std::regex const &TokenTemplate::getRegex(void) { return regTemplate; }