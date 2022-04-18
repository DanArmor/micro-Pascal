#ifndef __INC_ASTCLASSES_H
#define __INC_ASTCLASSES_H


// Предварительные объявления
class BinOpAST;
class NumberAST;
class UnOpAST;
class CompoundAST;
class AssignAST;
class VarAST;
class NoOpAST;
class ProgramAST;
class BlockAST;
class VarDeclAST;
class TypeSpecAST;
class ConstAST;
class StringAST;
class CallAST;
class ifAST;
class whileAST;

/**
 * @brief Интерфейс "посетителя"
 * @section IVisiterExmpl1 Пример
 * @include examples/example_visiter.cpp
 */
class IVisitor{
    public:
    virtual void visit(BinOpAST &node) = 0;
    virtual void visit(NumberAST &node) = 0;
    virtual void visit(UnOpAST &node) = 0;
    virtual void visit(CompoundAST &node) = 0;
    virtual void visit(AssignAST &node) = 0;
    virtual void visit(VarAST &node) = 0;
    virtual void visit(NoOpAST &node) = 0;
    virtual void visit(ProgramAST &node) = 0;
    virtual void visit(BlockAST &node) = 0;
    virtual void visit(VarDeclAST &node) = 0;
    virtual void visit(TypeSpecAST &node) = 0;
    virtual void visit(ConstAST &node) = 0;
    virtual void visit(StringAST &node) = 0;
    virtual void visit(CallAST &node) = 0;
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