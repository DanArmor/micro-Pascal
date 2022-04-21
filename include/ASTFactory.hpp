#ifndef __INC_ASTFACTORY_H
#define __INC_ASTFACTORY_H

#include "AST.hpp"
#include "Token.hpp"

/// @brief Простая фабрика AST-узлов
/// Возвращает узел по токену и аргументам, переданным слева-направо, сверху-вниз
class ASTFactory{
    public:

    static AST *createAST(Token token);

    static AST *createAST(Token token, AST *first);

    static AST *createAST(Token token, AST *first, AST *second);

    static AST *createAST(Token token, AST *first, AST *second, AST *third);

    static AST *createAST(Token token, std::vector<AST*> params);

    static AST *createAST(Token token, std::vector<AST*> constsDecls, std::vector<AST*> varDecls, AST *compound);

};


#endif
