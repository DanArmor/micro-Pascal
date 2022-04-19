#ifndef __INC_AST_H
#define __INC_AST_H

#include <iostream>
#include <vector>
#include <memory>

#include "Token.hpp"

#include "Visitor.hpp"
#include "ASTclasses.hpp"

/// @brief Узел, содержащий бинарную операцию
class BinOpAST : public AST{
    public:

    BinOpAST(Token token);
    BinOpAST(AST *left, Token token, AST *right);
    ~BinOpAST() = default;

    void accept(IVisitor &visitor);

    std::unique_ptr<AST> left;
    std::unique_ptr<AST> right;
};

/// @brief Узел, содержащий унарную операцию
class UnOpAST : public AST{
    public:

    UnOpAST(Token token);
    UnOpAST(Token token, AST *down);
    ~UnOpAST() = default;

    void accept(IVisitor &visitor);

    std::unique_ptr<AST> down;
};

/// @brief Узел, содержащий целое число
class NumberAST : public AST{
    public:

    NumberAST(Token token);
    ~NumberAST() = default;

    void accept(IVisitor &visitor);
};

/// @brief BEGIN END блоки
class CompoundAST : public AST{
    public:

    CompoundAST();
    ~CompoundAST() = default;

    void addChild(AST *child);

    void accept(IVisitor &visitor);

    std::vector<std::unique_ptr<AST>> children;

};

/// @brief Узел, содержащий бинарную операцию
class AssignAST : public AST{
    public:

    AssignAST(Token token);
    AssignAST(AST *var, Token token, AST *value);
    ~AssignAST() = default;

    void accept(IVisitor &visitor);

    std::unique_ptr<AST> var;
    std::unique_ptr<AST> value;
};

/// @brief Узел, содержащий переменную
class VarAST : public AST{
    public:

    VarAST(Token token);
    VarAST(Token token, int value);
    ~VarAST() = default;

    void accept(IVisitor &visitor);

    int value;
};

/// @brief Узел, отображающий пустовые выражение, отсутствие операций
class NoOpAST : public AST{
    public:

    NoOpAST(Token token);
    ~NoOpAST() = default;

    void accept(IVisitor &visitor);
};

/// @todo Вырезать из AST наследников лишние конструкторы

/// @brief Узел, отображающий начало программы
class ProgramAST : public AST{
    public:

    ProgramAST(Token name, ASTptr block);
    ~ProgramAST() = default;
    void accept(IVisitor &visitor);

    Token name;
    std::unique_ptr<AST> block;
};

class BlockAST : public AST{
    public:
    BlockAST(std::vector<AST*> consts, std::vector<AST*> declarations, ASTptr compound);
    ~BlockAST() = default;
    void accept(IVisitor &visitor);

    std::vector<std::unique_ptr<AST>> consts;
    std::vector<std::unique_ptr<AST>> declarations;
    std::unique_ptr<AST> compound;
};

class VarDeclAST : public AST{
    public:
    VarDeclAST(ASTptr var, std::shared_ptr<AST> type);
    ~VarDeclAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<AST> var;
    std::shared_ptr<AST> type;
};

class TypeSpecAST : public AST{
    public:
    TypeSpecAST(Token token);
    ~TypeSpecAST() = default;
    void accept(IVisitor &visitor);
};

class ConstAST : public AST{
    public:
    ConstAST(ASTptr constName, ASTptr constValue);
    ~ConstAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<AST> constName;
    std::unique_ptr<AST> constValue;
};

class StringAST : public AST{
    public:
    StringAST(Token token);
    ~StringAST() = default;
    void accept(IVisitor &visitor);
};

class CallAST : public AST{
    public:
    CallAST(Token token, std::vector<AST*> params);
    ~CallAST() = default;
    void accept(IVisitor &visitor);

    std::vector<std::unique_ptr<AST>> params;
};

class ifAST : public AST{
    public:
    ifAST(ASTptr condition, ASTptr body, ASTptr elseBody);
    ~ifAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<AST> condition;
    std::unique_ptr<AST> body;
    std::unique_ptr<AST> elseBody;
};

class whileAST : public AST{
    public:
    whileAST(ASTptr condition, ASTptr body);
    ~whileAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<AST> condition;
    std::unique_ptr<AST> body;
};


#endif