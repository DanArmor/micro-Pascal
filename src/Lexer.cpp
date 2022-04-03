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

std::string Lexer::getTrimmed(std::string str){
    int s=str.find_first_not_of(" \t");
    int e=str.find_last_not_of(" \t");

    // if do find real content
    if (s!=-1 && e!=-1)
        return str.substr(s, e-s+1);

    return "";
}

/**
 * @brief Анализирует файл
 * @param fileName имя файла для анализа
 */
List<Token> const &Lexer::analyzeFile(std::string fileName){
    std::fstream file(fileName, std::ios::in);

    std::stringstream text;
    std::string str;
    while(std::getline(file, str)){
        if(getTrimmed(str).size() == 0){
            wordsInLine.push_back(0);
            currLine++;
            continue;
        }
        wordsInLine.push_back(0);
        std::stringstream sup(" " + str);
        text << sup.rdbuf();
        std::stringstream sup2(str);
        std::string word;
        while(sup2 >> word)
            wordsInLine[currLine]++;
        currLine++;
    }
    currLine = 0;
    file.close();
    return analyzeProgramText(text);
}


Token Lexer::getToken(std::string &str){
    std::smatch m;
    Token toOutput("", IToken::Type::ERROR);
    for(auto temp : tokenTemplates){
        if(std::regex_search(str, m, temp.getRegex()), std::regex_constants::match_not_bol){
            if(m.str().size() > toOutput.getStr().size()){
                toOutput = Token(m.str(), temp.getType(), currLine);
            }
        }
    }
    str = str.substr(toOutput.getStr().size(), str.size());
    return toOutput;
}

void Lexer::checkLine(void){
    if(currLine < wordsInLine.size()){
        if(wordsInLine[currLine] == 0){
            while(currLine < wordsInLine.size() && wordsInLine[currLine] == 0)
                currLine++;
            return;
        }
        wordsInLine[currLine]--;
        while(currLine < wordsInLine.size() && wordsInLine[currLine] == 0)
            currLine++;
    }
}

void Lexer::skipComments(std::stringstream &text, std::string &str){
    bool isSkipping = true;
    do{
        while(isSkipping && str.size() != 0){
            Token token = getToken(str);
            if(token.getType() == IToken::RBRACE)
                isSkipping = false;
            else if(token.getType() == IToken::ERROR){
                str.erase(0, 1);
            }
        }
        if(isSkipping)
            checkLine();
    }while(isSkipping && (text >> str));
}

Token Lexer::collectString(std::stringstream &text,  std::string &strIn){
    stringsCount++;

    std::size_t startPos = 0;
    std::size_t endPos = 0;
    for(std::size_t i = 0; i < stringsCount; i++){
        startPos = text.str().find("'", endPos+1);
        endPos = text.str().find("'", startPos+1);
    }

    std::string tokenStr = text.str().substr(startPos, endPos-startPos+1);
 
    bool isSkipping = true;
    Token toOutput;
    do{
        while(isSkipping && strIn.size() != 0){
            Token token = getToken(strIn);
            if(token.getType() == IToken::STRING_BOUND){
                isSkipping = false;
                toOutput = {tokenStr, IToken::STRING_CONST, currLine};
            }
            else if(token.getType() == IToken::ERROR){
                strIn.erase(0, 1);
            }
        }
        if(isSkipping)
            checkLine();
    }while(isSkipping && (text >> strIn));
    return toOutput;
}

/**
 * @brief Анализирует текст программы
 * @param text строковый поток, представляющий программу
 */
List<Token> const &Lexer::analyzeProgramText(std::stringstream &text){
    std::string str;
    while(text >> str){
        while(str.size() != 0){
            Token toOutput = getToken(str);
            if(toOutput.getStr().size() == 0)
                throw(std::invalid_argument(fmt::format("Встречен неизвестный токен: {}", magic_enum::enum_name(toOutput.getType()))));
            if(toOutput.getType() == IToken::LBRACE){
                skipComments(text, str);
                continue;
            }
            if(toOutput.getType() == IToken::STRING_BOUND){
                toOutput = collectString(text, str);
            }
            //std::cout << fmt::format("Token: {} Type: {}  Line: {}\n", toOutput.getStr(), magic_enum::enum_name(toOutput.getType()), toOutput.line()+1);
            tokens.pushBack(toOutput);
        }
        checkLine();
    }
    tokens.pushBack({"$", IToken::ENDOFSTREAM});
    return tokens;
}