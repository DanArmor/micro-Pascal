#ifndef __INC_ASTCLASSES_H
#define __INC_ASTCLASSES_H

/**
 * @file ASTclasses.hpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Базовые классы ДАС: абстрактный посетитель и абстрактный узел
 * @version 1.0
 */

#include "Token.hpp"

// Предварительные объявления для посетителя
class ProgramAST;
class FunctionAST;
class BlockAST;
class VarDeclAST;
class TypeSpecAST;
class ArrSpecAST;
class ConstAST;
class CompoundAST;
class NumberAST;
class StringAST;
class BinOpAST;
class UnOpAST;
class NoOpAST;
class AssignAST;
class VarAST;
class SelectAST;
class CallAST;
class ReturnAST;
class IfAST;
class WhileAST;
class ForAST;
class IterationAST;

/// @brief Интерфейс "посетителя"
class IVisitor{
    public:
    virtual void visit(ProgramAST &node) = 0;
    virtual void visit(FunctionAST &node) = 0;
    virtual void visit(BlockAST &node) = 0;
    virtual void visit(VarDeclAST &node) = 0;
    virtual void visit(TypeSpecAST &node) = 0;
    virtual void visit(ArrSpecAST &node) = 0;
    virtual void visit(ConstAST &node) = 0;
    virtual void visit(CompoundAST &node) = 0;
    virtual void visit(NumberAST &node) = 0;
    virtual void visit(StringAST &node) = 0;
    virtual void visit(BinOpAST &node) = 0;
    virtual void visit(UnOpAST &node) = 0;
    virtual void visit(NoOpAST &node) = 0;
    virtual void visit(AssignAST &node) = 0;
    virtual void visit(VarAST &node) = 0;
    virtual void visit(SelectAST &node) = 0;
    virtual void visit(CallAST &node) = 0;
    virtual void visit(ReturnAST &node) = 0;
    virtual void visit(IfAST &node) = 0;
    virtual void visit(WhileAST &node) = 0;
    virtual void visit(IterationAST &node) = 0;
    virtual void visit(ForAST &node) = 0;
};

/// @brief Базовый класс узлов абстрактного синтаксического дерева
class IAST{
    public:

    IAST();
    IAST(Token token);
    virtual ~IAST() = default;

    virtual void accept(IVisitor &visitor) = 0;

    Token token;
};

#endif