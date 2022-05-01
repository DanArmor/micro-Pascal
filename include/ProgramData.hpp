#ifndef __INC_PROGRAM_DATA_H
#define __INC_PROGRAM_DATA_H

#include <map>
#include <string>

#include "Token.hpp"
#include "SyntExp.hpp"

class ProgramData{
    public:

    void addProgName(Token token){
        programName[token.getStr()] = token;
    }

    void addFunc(Token token){
        if(functions.count(token.getStr()) != 0)
            throw SyntaxException(token, "Повторное объявление подпрограммы! ");
        functions[token.getStr()] = token;
    }

    void checkVar(Token token){
        if(vars.count(token.getStr()) == 0)
            throw SyntaxException(token, "Использование до объявления! ");
    }

    void checkFunc(Token token){
        if(functions.count(token.getStr()) == 0)
            throw SyntaxException(token, "Использование до объявления! ");
    }

    private:
    List<Token> tokens;
    std::map<std::string, Token> vars;
    std::map<std::string, Token> consts;
    std::map<std::string, Token> functions;
    std::map<std::string, Token> programName;
};

#endif