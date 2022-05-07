#ifndef __INC_LEXER_H
#define __INC_LEXER_H

#include <fstream>
#include <string>

#include "List.hpp"
#include "Token.hpp"

/**
 * @file Lexer.hpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Лексер
 * @version 1.0
 */

/// @brief Класс Лексер, разбивающий текст программы на токены
class Lexer {
   public:
    /**
     * @brief Устанавливает шаблоны токенов для разбора лексером
     * @param templates список шаблонов
     */
    void setTemplates(List<TokenTemplate> const &templates);

    /// @brief Возвращает строку, у которой убраны пустые символы в начале и
    /// конце
    std::string getTrimmed(std::string const &str);

    std::string getText(void);

    /**
     * @brief Анализирует файл
     * @param fileName имя файла для анализа
     */
    List<Token> analyzeFile(std::string const &fileName);

    /**
     * @brief Анализирует текст программы
     * @param text строковый поток, представляющий программу
     */
    List<Token> analyzeProgramText(std::string const &text);

   private:
    /// @brief Шаблоны токенов, которые может определять лексер
    List<TokenTemplate> tokenTemplates;
    std::string progText;
    List<Token> tokens;
};

#endif