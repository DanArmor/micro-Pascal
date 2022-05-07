#ifndef __INC_AST_H
#define __INC_AST_H

/**
 * @file AST.hpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Узлы ДАС
 * @version 1.0
 */

#include <memory>
#include <vector>

#include "ASTclasses.hpp"
#include "Token.hpp"

/// @brief Узел, отображающий программу
class ProgramAST : public IAST {
   public:
    ProgramAST(Token name, std::vector<std::unique_ptr<IAST>> functions,
               std::unique_ptr<IAST> block);
    ~ProgramAST() = default;
    void accept(IVisitor &visitor);

    Token name;
    std::vector<std::unique_ptr<IAST>> functions;
    std::unique_ptr<IAST> block;
};

/// @brief Узел, представляющий определение функции
class FunctionAST : public IAST {
   public:
    FunctionAST(Token token, std::vector<std::unique_ptr<IAST>> params,
                std::unique_ptr<IAST> returnType, std::unique_ptr<IAST> body);
    ~FunctionAST() = default;
    void accept(IVisitor &visitor);

    Token name;
    std::vector<std::unique_ptr<IAST>> params;
    std::unique_ptr<IAST> returnType;
    std::unique_ptr<IAST> body;
};

/// @brief Узел, отображающий блок (константы, переменные, составной оператор)
class BlockAST : public IAST {
   public:
    BlockAST(Token token, std::vector<std::unique_ptr<IAST>> consts,
             std::vector<std::unique_ptr<IAST>> declarations,
             std::unique_ptr<IAST> compound);
    ~BlockAST() = default;
    void accept(IVisitor &visitor);

    std::vector<std::unique_ptr<IAST>> consts;
    std::vector<std::unique_ptr<IAST>> declarations;
    std::unique_ptr<IAST> compound;
};

/// @brief Узел, представляющий определение переменной
class VarDeclAST : public IAST {
   public:
    VarDeclAST(Token token, std::unique_ptr<IAST> var,
               std::unique_ptr<IAST> type);
    ~VarDeclAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<IAST> var;
    std::unique_ptr<IAST> type;
};

/// @brief Узел, представляющий определение типа
class TypeSpecAST : public IAST {
   public:
    explicit TypeSpecAST(Token token);
    ~TypeSpecAST() = default;
    void accept(IVisitor &visitor);
};

/// @brief Узел, представляющий определение типа массива
class ArrSpecAST : public IAST {
   public:
    ArrSpecAST(Token arrTok, Token lHandTok, Token rHandTok,
               std::unique_ptr<IAST> subType);
    ~ArrSpecAST() = default;
    void accept(IVisitor &visitor);

    Token lHandTok;
    Token rHandTok;
    std::unique_ptr<IAST> subType;
};

/// @brief Узел, представляющий константу
class ConstAST : public IAST {
   public:
    ConstAST(Token token, std::unique_ptr<IAST> constName,
             std::unique_ptr<IAST> constValue);
    ~ConstAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<IAST> constName;
    std::unique_ptr<IAST> constValue;
};

/// @brief Узел, представляющий составной операторор
class CompoundAST : public IAST {
   public:
    explicit CompoundAST(Token token);
    ~CompoundAST() = default;

    void addChild(std::unique_ptr<IAST> child);

    void accept(IVisitor &visitor);

    std::vector<std::unique_ptr<IAST>> children;
};

/// @brief Узел, содержащий целое число
class NumberAST : public IAST {
   public:
    explicit NumberAST(Token token);
    ~NumberAST() = default;

    void accept(IVisitor &visitor);
};

/// @brief Узел, содержащий строку
class StringAST : public IAST {
   public:
    explicit StringAST(Token token);
    ~StringAST() = default;
    void accept(IVisitor &visitor);
};

/// @brief Узел, содержащий бинарную операцию
class BinOpAST : public IAST {
   public:
    BinOpAST(Token token, std::unique_ptr<IAST> left,
             std::unique_ptr<IAST> right);
    ~BinOpAST() = default;

    void accept(IVisitor &visitor);

    std::unique_ptr<IAST> left;
    std::unique_ptr<IAST> right;
};

/// @brief Узел, содержащий унарную операцию
class UnOpAST : public IAST {
   public:
    UnOpAST(Token token, std::unique_ptr<IAST> down);
    ~UnOpAST() = default;

    void accept(IVisitor &visitor);

    std::unique_ptr<IAST> down;
};

/// @brief Узел, отображающий пустовые выражение, отсутствие операций
class NoOpAST : public IAST {
   public:
    explicit NoOpAST(Token token);
    ~NoOpAST() = default;

    void accept(IVisitor &visitor);
};

/// @brief Узел, содержащий операцию присваивания
class AssignAST : public IAST {
   public:
    AssignAST(Token token, std::unique_ptr<IAST> var,
              std::unique_ptr<IAST> value);
    ~AssignAST() = default;

    void accept(IVisitor &visitor);

    std::unique_ptr<IAST> var;
    std::unique_ptr<IAST> value;
};

/// @brief Узел, содержащий переменную
class VarAST : public IAST {
   public:
    explicit VarAST(Token token);
    ~VarAST() = default;

    void accept(IVisitor &visitor);
};

/// @brief Узел, преставляющий выбор элемента массива
class SelectAST : public IAST {
   public:
    SelectAST(Token token, std::unique_ptr<IAST> from,
              std::unique_ptr<IAST> index);
    ~SelectAST() = default;

    void accept(IVisitor &visitor);

    std::unique_ptr<IAST> from;
    std::unique_ptr<IAST> index;
};

/// @brief Узел, преставляющий вызов функции
class CallAST : public IAST {
   public:
    CallAST(Token token, std::vector<std::unique_ptr<IAST>> params);
    ~CallAST() = default;
    void accept(IVisitor &visitor);

    std::vector<std::unique_ptr<IAST>> params;
};

/// @brief Узел, содержащий return statement
class ReturnAST : public IAST {
   public:
    ReturnAST(Token token, std::unique_ptr<IAST> toReturn);
    ~ReturnAST() = default;

    void accept(IVisitor &visitor);

    std::unique_ptr<IAST> toReturn;
};

/// @brief Узел, преставляющий if statement
class IfAST : public IAST {
   public:
    IfAST(Token token, std::unique_ptr<IAST> condition,
          std::unique_ptr<IAST> body, std::unique_ptr<IAST> elseBody);
    ~IfAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<IAST> condition;
    std::unique_ptr<IAST> body;
    std::unique_ptr<IAST> elseBody;
};

/// @brief Узел, преставляющий while statement
class WhileAST : public IAST {
   public:
    WhileAST(Token token, std::unique_ptr<IAST> condition,
             std::unique_ptr<IAST> body);
    ~WhileAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<IAST> condition;
    std::unique_ptr<IAST> body;
};

/// @brief Узел, преставляющий for statement
class ForAST : public IAST {
   public:
    ForAST(Token token, std::unique_ptr<IAST> iterSt,
           std::unique_ptr<IAST> body);
    ~ForAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<IAST> iterSt;
    std::unique_ptr<IAST> body;
};

/// @brief Узел, представляющий заголовок цикла for
class IterationAST : public IAST {
   public:
    IterationAST(Token token, std::unique_ptr<IAST> assign,
                 std::unique_ptr<IAST> condition,
                 std::unique_ptr<IAST> postAction);
    ~IterationAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<IAST> assign;
    std::unique_ptr<IAST> condition;
    std::unique_ptr<IAST> postAction;
};

#endif