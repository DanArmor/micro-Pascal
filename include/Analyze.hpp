#ifndef _INC_ANALYZE_H
#define _INC_ANALYZE_H

/**
 * @file Analyze.hpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Функции анализа кода на языке Pascal
 * @version 1.0
 */

#include <string>

#include "List.hpp"
#include "Token.hpp"

/// @brief Функции анализа программ на языке Pascal
namespace Analyze {

/// @brief Выводит в стандартный поток вывода подсвеченный текст программы
void highlight(std::string text, List<Token> tokens);

/// @brief Анализ файла inName и вывод в outName .dot описания ДАС. Так же
/// выводит в стандартный поток вывода подсвеченный текст программы
void analyzeFile(std::string inName, std::string outName);

/// @brief Анализ файла inName и вывод в "output.svg" .dot описания ДАС. Так же
/// выводит в стандартный поток вывода подсвеченный текст программы
void analyzeFile(std::string inName);

}  // namespace Analyze

#endif