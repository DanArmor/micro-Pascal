#include <map>
#include <string>
#include <fmt/format.h>
#include <iostream>
#include "magic_enum.hpp"
#include <algorithm>

#include "Lexer.hpp"


/**
 * @brief Добавляет в шаблоны для определения токена новый шаблон
 * @param regStr regex-строка для определения токена 
 * @param type тип токена
 */
void Lexer::addTokenTemp(std::string regStr, IToken::Type type){
    tokenTemplates.pushBack(TokenTemplate(regStr, type));
}

/**
 * @brief Добавляет в шаблоны для определения токена новый шаблон
 * @param tokenTemp шаблон токена
 */
void Lexer::addTokenTemp(TokenTemplate tokenTemp){
    tokenTemplates.pushBack(tokenTemp);
}

void Lexer::setTemplates(List<TokenTemplate> const &templates){
    tokenTemplates = templates;
}

/**
 * @brief Анализирует файл
 * @param fileName имя файла для анализа
 */
List<Token> const &Lexer::analyzeFile(std::string fileName){
    std::fstream file(fileName, std::ios::in);

    if(!file){
        throw std::invalid_argument("Невозможно открыть файл!\n");
    }

    std::stringstream text;
    text << file.rdbuf();
    file.close();
    return analyzeProgramText(text.str());
}

std::string Lexer::getTrimmed(std::string str){
    int s=str.find_first_not_of(" \t\n");
    int e=str.find_last_not_of(" \t");

    // if do find real content
    if (s!=-1 && e!=-1)
        return str.substr(s, e-s+1);

    return "";
}

/**
 * @brief Анализирует текст программы
 * @param text строковый поток, представляющий программу
 */
List<Token> const &Lexer::analyzeProgramText(std::string const &text){
    std::map<std::size_t, std::pair<std::string, IToken::Type>> result;
    std::size_t position = 0;
    std::size_t len = 0;
    std::size_t offset = 0;
    Token matched;
    while(true){
        matched = {"", IToken::ERROR};
        for(auto pat = tokenTemplates.begin(); pat != tokenTemplates.end(); ++pat){
            std::regex r((*pat).getRegex());
            auto wordsBegin = std::sregex_iterator(text.begin()+position, text.end(), r);
            auto wordsEnd = std::sregex_iterator();
            if(wordsBegin != wordsEnd){
                //std::cout << wordsBegin->str() << "\n";
                std::string word = getTrimmed(wordsBegin->str());
                if(wordsBegin->str().size() > len){
                    len = wordsBegin->str().size();
                    offset = wordsBegin->position();
                    matched = {word, (*pat).getType(), 0};
                }
            }
        }
        //std::cout << position << " " << len << " " << offset << " ||| " << text.size() << "\n";
        if(matched.getType() != IToken::NEWLINE && matched.getStr().size() == 0)
            throw std::invalid_argument("Неизвестный токен");
        position = position + len + offset;
        //std::cout << matched.getInfo() << "\n\n";
        if(matched.getType() != IToken::NEWLINE && matched.getType() != IToken::COMMENT)
            tokens.pushBack(matched);
        if(position >= text.size())
            break;
        len = 0;
        offset = 0;
    }
    tokens.pushBack({"$", IToken::ENDOFSTREAM});
    return tokens;
}