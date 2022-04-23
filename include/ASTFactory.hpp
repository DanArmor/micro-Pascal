#ifndef __INC_ASTFACTORY_H
#define __INC_ASTFACTORY_H

#include "AST.hpp"
#include "Token.hpp"

/// @brief Простая фабрика AST-узлов
/// Возвращает узел по токену и аргументам, переданным слева-направо, сверху-вниз
class ASTFactory{
    public:

    static std::unique_ptr<AST> createAST(Token token);

    static std::unique_ptr<AST> createAST(Token token, std::unique_ptr<AST> first);

    static std::unique_ptr<AST> createAST(Token token, std::unique_ptr<AST> first, std::unique_ptr<AST> second);

    static std::unique_ptr<AST> createAST(Token token, std::unique_ptr<AST> first, std::unique_ptr<AST> second, std::unique_ptr<AST> third);

    static std::unique_ptr<AST> createAST(Token token, std::vector<std::unique_ptr<AST>> params);

    static std::unique_ptr<AST> createAST(Token token, std::vector<std::unique_ptr<AST>> params, std::unique_ptr<AST> first);

    static std::unique_ptr<AST> createAST(Token token, std::vector<std::unique_ptr<AST>> params, std::unique_ptr<AST> first, std::unique_ptr<AST> second);

    static std::unique_ptr<AST> createAST(Token token, 
    std::vector<std::unique_ptr<AST>> constsDecls, std::vector<std::unique_ptr<AST>> varDecls, std::unique_ptr<AST> compound);

};


#endif
