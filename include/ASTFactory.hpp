#ifndef __INC_ASTFACTORY_H
#define __INC_ASTFACTORY_H

/**
 * @file ASTFactory.hpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Простая Фабрика для AST-узлов
 * @version 1.0
 */

#include "ASTclasses.hpp"
#include "Token.hpp"

/// @brief Простая фабрика AST-узлов
/// Возвращает узел по основному и дополнительному типу токена
/// и аргументам, переданным так, если бы они
/// следовали слева-направо, сверху-вниз в анализируемом коде
class ASTFactory {
   public:
    static std::unique_ptr<IAST> createAST(Token token);

    static std::unique_ptr<IAST> createAST(Token token,
                                           std::unique_ptr<IAST> first);

    static std::unique_ptr<IAST> createAST(Token token,
                                           std::unique_ptr<IAST> first,
                                           std::unique_ptr<IAST> second);

    static std::unique_ptr<IAST> createAST(Token token,
                                           std::unique_ptr<IAST> first,
                                           std::unique_ptr<IAST> second,
                                           std::unique_ptr<IAST> third);

    static std::unique_ptr<IAST> createAST(
        Token token, std::vector<std::unique_ptr<IAST>> params);

    static std::unique_ptr<IAST> createAST(
        Token token, std::vector<std::unique_ptr<IAST>> params,
        std::unique_ptr<IAST> first);

    static std::unique_ptr<IAST> createAST(
        Token token, std::vector<std::unique_ptr<IAST>> params,
        std::unique_ptr<IAST> first, std::unique_ptr<IAST> second);

    static std::unique_ptr<IAST> createAST(
        Token token, std::vector<std::unique_ptr<IAST>> constsDecls,
        std::vector<std::unique_ptr<IAST>> varDecls,
        std::unique_ptr<IAST> compound);

    static std::unique_ptr<IAST> createAST(Token firstTok, Token secondTok,
                                           Token thirdTok,
                                           std::unique_ptr<IAST> first);

    /// @brief Возвращает "глубокую копию" указателя на тип typePtr
    static std::unique_ptr<IAST> createCopyOfType(IAST *typePtr);
};

#endif
