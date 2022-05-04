#ifndef __INC_LEXER_H
#define __INC_LEXER_H

#include <string>
#include <fstream>

#include "Token.hpp"
#include "List.hpp"

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
    void addTokenTemp(std::string const &regStr, IToken::Type const type, IToken::AdvType const advType);

    /**
     * @brief Добавляет в шаблоны для определения токена новый шаблон
     * @param tokenTemp шаблон токена
     */
    void addTokenTemp(TokenTemplate const &tokenTemp);

    /**
     * @brief Устанавливает шаблоны токенов для разбора лексером
     * @param templates список шаблонов
     */
    void setTemplates(List<TokenTemplate> const &templates);

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