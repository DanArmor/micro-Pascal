#ifndef __INC_AST_H
#define __INC_AST_H

#include <vector>
#include <memory>

#include "Token.hpp"

#include "Visitor.hpp"
#include "ASTclasses.hpp"

/// @brief Узел, отображающий начало программы
class ProgramAST : public AST{
    public:

    ProgramAST(Token name, std::vector<std::unique_ptr<AST>> functions, std::unique_ptr<AST> block);
    ~ProgramAST() = default;
    void accept(IVisitor &visitor);

    Token name;
    std::vector<std::unique_ptr<AST>> functions;
    std::unique_ptr<AST> block;
};

/// @brief Узел, отображающий блок (константы, переменные, инструкции)
class BlockAST : public AST{
    public:

    BlockAST(Token token, std::vector<std::unique_ptr<AST>> consts, std::vector<std::unique_ptr<AST>> declarations, std::unique_ptr<AST> compound);
    ~BlockAST() = default;
    void accept(IVisitor &visitor);

    std::vector<std::unique_ptr<AST>> consts;
    std::vector<std::unique_ptr<AST>> declarations;
    std::unique_ptr<AST> compound;
};


/// @brief Узел, содержащий бинарную операцию
class BinOpAST : public AST{
    public:

    BinOpAST(Token token, std::unique_ptr<AST> left, std::unique_ptr<AST> right);
    ~BinOpAST() = default;

    void accept(IVisitor &visitor);

    std::unique_ptr<AST> left;
    std::unique_ptr<AST> right;
};

/// @brief Узел, содержащий унарную операцию
class UnOpAST : public AST{
    public:

    UnOpAST(Token token, std::unique_ptr<AST> down);
    ~UnOpAST() = default;

    void accept(IVisitor &visitor);

    std::unique_ptr<AST> down;
};

/// @brief Узел, содержащий целое число
class NumberAST : public AST{
    public:

    explicit NumberAST(Token token);
    ~NumberAST() = default;

    void accept(IVisitor &visitor);
};

/// @brief BEGIN END блоки
class CompoundAST : public AST{
    public:

    explicit CompoundAST(Token token);
    ~CompoundAST() = default;

    void addChild(std::unique_ptr<AST> child);

    void accept(IVisitor &visitor);

    std::vector<std::unique_ptr<AST>> children;
};

/// @brief Узел, содержащий бинарную операцию
class AssignAST : public AST{
    public:

    AssignAST(Token token, std::unique_ptr<AST> var, std::unique_ptr<AST> value);
    ~AssignAST() = default;

    void accept(IVisitor &visitor);

    std::unique_ptr<AST> var;
    std::unique_ptr<AST> value;
};

/// @brief Узел, содержащий переменную
class VarAST : public AST{
    public:

    explicit VarAST(Token token);
    ~VarAST() = default;

    void accept(IVisitor &visitor);
};

/// @brief Узел, отображающий пустовые выражение, отсутствие операций
class NoOpAST : public AST{
    public:

    explicit NoOpAST(Token token);
    ~NoOpAST() = default;

    void accept(IVisitor &visitor);
};


class VarDeclAST : public AST{
    public:

    VarDeclAST(Token token, std::unique_ptr<AST> var, std::unique_ptr<AST> type);
    ~VarDeclAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<AST> var;
    std::unique_ptr<AST> type;
};

class TypeSpecAST : public AST{
    public:

    explicit TypeSpecAST(Token token);
    ~TypeSpecAST() = default;
    void accept(IVisitor &visitor);
};

class ArrSpecAST : public AST{
    public:

    ArrSpecAST(Token arrTok, Token lHandTok, Token rHandTok, std::unique_ptr<AST> subType);
    ~ArrSpecAST() = default;
    void accept(IVisitor &visitor);

    Token lHandTok;
    Token rHandTok;
    std::unique_ptr<AST> subType;
};

class ConstAST : public AST{
    public:

    ConstAST(Token token, std::unique_ptr<AST> constName, std::unique_ptr<AST> constValue);
    ~ConstAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<AST> constName;
    std::unique_ptr<AST> constValue;
};

class StringAST : public AST{
    public:

    explicit StringAST(Token token);
    ~StringAST() = default;
    void accept(IVisitor &visitor);
};

class CallAST : public AST{
    public:

    CallAST(Token token, std::vector<std::unique_ptr<AST>> params);
    ~CallAST() = default;
    void accept(IVisitor &visitor);

    std::vector<std::unique_ptr<AST>> params;
};

class IfAST : public AST{
    public:

    IfAST(Token token, std::unique_ptr<AST> condition, std::unique_ptr<AST> body, std::unique_ptr<AST> elseBody);
    ~IfAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<AST> condition;
    std::unique_ptr<AST> body;
    std::unique_ptr<AST> elseBody;
};

class WhileAST : public AST{
    public:

    WhileAST(Token token, std::unique_ptr<AST> condition, std::unique_ptr<AST> body);
    ~WhileAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<AST> condition;
    std::unique_ptr<AST> body;
};

class IterationAST : public AST{
    public:

    IterationAST(Token token, std::unique_ptr<AST> assign, std::unique_ptr<AST> condition, std::unique_ptr<AST> postAction);
    ~IterationAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<AST> assign;
    std::unique_ptr<AST> condition;
    std::unique_ptr<AST> postAction;
};

class ForAST : public AST{
    public:

    ForAST(Token token, std::unique_ptr<AST> iterSt, std::unique_ptr<AST> body);
    ~ForAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<AST> iterSt;
    std::unique_ptr<AST> body;
};

class FunctionAST : public AST{
    public:

    FunctionAST(Token token, std::vector<std::unique_ptr<AST>> params, std::unique_ptr<AST> returnType, std::unique_ptr<AST> body);
    ~FunctionAST() = default;
    void accept(IVisitor &visitor);

    std::string name;
    std::vector<std::unique_ptr<AST>> params;
    std::unique_ptr<AST> returnType;
    std::unique_ptr<AST> body;
};

/// @brief Узел, содержащий return statement
class ReturnAST : public AST{
    public:

    ReturnAST(Token token, std::unique_ptr<AST> toReturn);
    ~ReturnAST() = default;

    void accept(IVisitor &visitor);

    std::unique_ptr<AST> toReturn;
};

/// @brief Узел, преставляющий выбор элемента в массиве
class SelectAST : public AST{
    public:

    SelectAST(Token token, std::unique_ptr<AST> from, std::unique_ptr<AST> index);
    ~SelectAST() = default;

    void accept(IVisitor &visitor);

    std::unique_ptr<AST> from;
    std::unique_ptr<AST> index;
};

#endif