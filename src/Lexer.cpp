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

/**
 * @brief Анализирует текст программы
 * @param text строковый поток, представляющий программу
 */
List<Token> const &Lexer::analyzeProgramText(std::string const &text){
    std::map<std::size_t, std::pair<std::string, IToken::Type>> result;
    for(auto pat = tokenTemplates.begin(); pat != tokenTemplates.end(); ++pat){
        std::regex r((*pat).getRegex());
        auto wordsBegin = std::sregex_iterator(text.begin(), text.end(), r);
        auto wordsEnd = std::sregex_iterator();
        for(auto it = wordsBegin; it != wordsEnd; ++it){
            result[it->position()] = std::make_pair(it->str(), (*pat).getType());
        }
    }
    bool isStrConst = false;
    std::size_t linePosition = 0;
    for(auto match = result.begin(); match != result.end(); ++match){
        switch(match->second.second){
            case IToken::STRING_BOUND :
                if(isStrConst){
                    isStrConst = false;
                } else{
                    throw std::invalid_argument("Незакрытая кавычка строки!");
                }
                break;
            case IToken::STRING_CONST :
                tokens.pushBack({match->second.first, match->second.second, linePosition});
                isStrConst = true;
                break;
            case IToken::NEWLINE :
                linePosition++;
                break;
            default:
                if(!isStrConst)
                    tokens.pushBack({match->second.first, match->second.second, linePosition});
                break;
        }
    }
    tokens.pushBack({"$", IToken::ENDOFSTREAM});
    for(auto p : tokens){
        std::cout << p.getInfo() << "\n\n";
    }
    return tokens;
}