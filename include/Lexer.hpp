#ifndef __INC_LEXER_H
#define __INC_LEXER_H

#include <string>
#include <fstream>

#include "Token.hpp"
#include "List.cpp"

/// @todo Изменить комментарии некоторых методов, где появились возвращаемые значения (в лексере)
/// @todo уборка, комментарии

/**
 * @file Lexer.cpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Лексер
 * @version 0.5
 * @date 2022-03-17
 */

/// @brief Класс Лексер, разбивающий текст программы на токены
class Lexer{
    public:

    /**
     * @brief Добавляет в шаблоны для определения токена новый шаблон
     * @param regStr regex-строка для определения токена 
     * @param type тип токена
     */
    void addTokenTemp(std::string regStr, IToken::Type type);

    /**
     * @brief Добавляет в шаблоны для определения токена новый шаблон
     * @param tokenTemp шаблон токена
     */
    void addTokenTemp(TokenTemplate tokenTemp);

    /**
     * @brief Устанавливает шаблоны токенов для разбора лексером
     * @param templates список шаблонов
     */
    void setTemplates(List<TokenTemplate> const &templates);

    /**
     * @brief Анализирует файл
     * @param fileName имя файла для анализа
     */
    List<Token> const &analyzeFile(std::string fileName);

    /**
     * @brief Анализирует текст программы
     * @param text строковый поток, представляющий программу
     */
    List<Token> const &analyzeProgramText(std::string const &text);

    private:
    /// @brief Шаблоны токенов, которые может определять лексер
    List<TokenTemplate> tokenTemplates;

    List<Token> tokens;
};

#endif