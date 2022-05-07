#ifndef __INC_SYNTEXP_H
#define __INC_SYNTEXP_H

/**
 * @file SyntExp.hpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Исключения анализатора
 * @version 1.0
 */

#include <stdexcept>
#include <string>

#include "Token.hpp"

/// @brief Базовое исключение анализатора
class AnalyzeException : public std::exception {
   public:
    AnalyzeException(Token errorTok, std::string msg);

    const char *what() const throw();

   protected:
    std::string msg;
    Token errorTok;
};

/// @brief Лексическое исключение
class LexerException : public AnalyzeException {
   public:
    LexerException(Token errorTok, std::string msg);
};

/// @brief Синтаксическое исключение
class SyntaxException : public AnalyzeException {
   public:
    SyntaxException(Token errorTok, std::string msg);
};

/// @brief Семантическое исключение
class SemanticException : public AnalyzeException {
   public:
    SemanticException(Token errorTok, std::string msg);
};

/// @brief Исключение типизирования
class TypeException : public SemanticException {
   public:
    TypeException(Token errorTok, std::string msg);
};

#endif