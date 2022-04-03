#ifndef __INC_ASTCLASSES_H
#define __INC_ASTCLASSES_H


// Предварительные объявления
class BinOp;
class Number;
class UnOp;
class Compound;
class Assign;
class Var;
class NoOp;
class ProgramAST;
class BlockAST;
class VarDeclaration;
class Type;
class ConstAST;
class StringAST;
class ProcedureCall;
class ifAST;
class whileAST;

/**
 * @brief Интерфейс "посетителя"
 * @section IVisiterExmpl1 Пример
 * @include examples/example_visiter.cpp
 */
class IVisitor{
    public:
    virtual void visit(BinOp &node) = 0;
    virtual void visit(Number &node) = 0;
    virtual void visit(UnOp &node) = 0;
    virtual void visit(Compound &node) = 0;
    virtual void visit(Assign &node) = 0;
    virtual void visit(Var &node) = 0;
    virtual void visit(NoOp &node) = 0;
    virtual void visit(ProgramAST &node) = 0;
    virtual void visit(BlockAST &node) = 0;
    virtual void visit(VarDeclaration &node) = 0;
    virtual void visit(Type &node) = 0;
    virtual void visit(ConstAST &node) = 0;
    virtual void visit(StringAST &node) = 0;
    virtual void visit(ProcedureCall &node) = 0;
    virtual void visit(ifAST &node) = 0;
    virtual void visit(whileAST &node) = 0;
};

/// @brief Базовый класс узлов абстрактного синтаксического дерева
class AST{
    public:

    //AST();
    AST(Token token);

    virtual void accept(IVisitor &visitor) = 0;

    Token token;
};

/// @brief Тип указателя на узел АСД
typedef AST* ASTptr;

#endif