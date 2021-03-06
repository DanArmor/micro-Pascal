#include "Lexer.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <string>

#include "SyntExp.hpp"
#include "magic_enum.hpp"

void Lexer::setTemplates(List<TokenTemplate> const &templates) {
    tokenTemplates = templates;
}

/**
 * @brief Анализирует файл
 * @param fileName имя файла для анализа
 */
List<Token> Lexer::analyzeFile(std::string const &fileName) {
    std::fstream file(fileName, std::ios::in);

    if (!file) {
        throw std::invalid_argument("Невозможно открыть файл!\n");
    }

    std::stringstream text;
    text << file.rdbuf();
    file.close();
    return analyzeProgramText(text.str());
}

std::string Lexer::getTrimmed(std::string const &str) {
    std::size_t const s = str.find_first_not_of(" \t\n");
    std::size_t const e = str.find_last_not_of(" \t");

    // Если не найдены
    if (s != std::string::npos && e != std::string::npos)
        return str.substr(s, e - s + 1);

    return "";
}

std::size_t countFromBegin(std::string const &str) {
    std::size_t const s = str.find_first_not_of(" \t\n");
    return s;
}

/**
 * @brief Анализирует текст программы
 * @param text строковый поток, представляющий программу
 */
List<Token> Lexer::analyzeProgramText(std::string const &text) {
    progText = text;

    std::size_t position = 0;
    std::size_t lineNum = 0;
    std::size_t inLineNum = 0;

    while (true) {
        std::size_t spacesFromBegin = 0;
        std::size_t len = 0;

        Token matched("", BaseToken::ERROR, BaseToken::UNKNOWN, lineNum, inLineNum,
                      position);
        for (auto pattern = tokenTemplates.begin();
             pattern != tokenTemplates.end(); ++pattern) {
            std::regex const r((*pattern).getRegex());
            auto wordsBegin =
                std::sregex_iterator(text.begin() + position, text.end(), r);
            auto wordsEnd = std::sregex_iterator();
            if (wordsBegin != wordsEnd) {
                std::string word = getTrimmed(wordsBegin->str());
                if (wordsBegin->str().size() > len) {
                    len = wordsBegin->str().size();
                    spacesFromBegin = countFromBegin(wordsBegin->str());
                    matched = {word,
                               (*pattern).getType(),
                               (*pattern).getAdvType(),
                               lineNum,
                               inLineNum + spacesFromBegin,
                               position + spacesFromBegin};
                }
            }
        }
        if (matched.getType() != BaseToken::NEWLINE &&
            matched.getStr().size() == 0)
            throw LexerException(matched, "Неизвестный токен!");
        position = position + len;
        inLineNum += len;

        if (matched.getType() != BaseToken::NEWLINE &&
            matched.getType() != BaseToken::COMMENT)
            tokens.pushBack(matched);
        if (position >= text.size()) break;

        if (matched.getType() == BaseToken::NEWLINE) {
            lineNum++;
            inLineNum = 0;
        } else if (matched.getType() == BaseToken::COMMENT) {
            std::size_t const linesInComment = std::count(
                matched.getStr().begin(), matched.getStr().end(), '\n');
            lineNum += linesInComment;
            if (linesInComment != 0) {
                inLineNum = matched.getStr().size() -
                            matched.getStr().find_last_of('\n') - 1;
            }
        }
    }
    Token endOfStream("$", BaseToken::ENDOFSTREAM, BaseToken::UNKNOWN);
    tokens.pushBack(endOfStream);
    return tokens;
}

std::string Lexer::getText(void) { return progText; }