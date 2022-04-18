#ifndef __INC_AST_H
#define __INC_AST_H

#include <vector>

#include "Token.hpp"

#include "Visitor.hpp"
#include "ASTclasses.hpp"

/// @brief Узел, содержащий бинарную операцию
class BinOpAST : public AST{
    public:

    BinOpAST(Token token);
    BinOpAST(AST *left, Token token, AST *right);

    void accept(IVisitor &visitor);

    AST *left;
    AST *right;
};

/// @brief Узел, содержащий унарную операцию
class UnOpAST : public AST{
    public:

    UnOpAST(Token token);
    UnOpAST(Token token, AST *down);

    void accept(IVisitor &visitor);

    AST *down;
};

/// @brief Узел, содержащий целое число
class NumberAST : public AST{
    public:

    NumberAST(Token token);

    void accept(IVisitor &visitor);
};

/// @brief BEGIN END блоки
class CompoundAST : public AST{
    public:

    CompoundAST();

    void addChild(ASTptr child);

    void accept(IVisitor &visitor);

    std::vector<ASTptr> children;

};

/// @brief Узел, содержащий бинарную операцию
class AssignAST : public AST{
    public:

    AssignAST(Token token);
    AssignAST(AST *var, Token token, AST *value);

    void accept(IVisitor &visitor);

    AST *var;
    AST *value;
};

/// @brief Узел, содержащий переменную
class VarAST : public AST{
    public:

    VarAST(Token token);
    VarAST(Token token, int value);

    void accept(IVisitor &visitor);

    int value;
};

/// @brief Узел, отображающий пустовые выражение, отсутствие операций
class NoOpAST : public AST{
    public:

    NoOpAST(Token token);

    void accept(IVisitor &visitor);
};

/// @todo Вырезать из AST наследников лишние конструкторы

/// @brief Узел, отображающий начало программы
class ProgramAST : public AST{
    public:

    ProgramAST(Token name, ASTptr block);
    void accept(IVisitor &visitor);

    Token name;
    AST *block;
};

class BlockAST : public AST{
    public:
    BlockAST(std::vector<ASTptr> consts, std::vector<ASTptr> declarations, ASTptr compound);
    void accept(IVisitor &visitor);

    std::vector<ASTptr> consts;
    std::vector<ASTptr> declarations;
    ASTptr compound;
};

class VarDeclAST : public AST{
    public:
    VarDeclAST(ASTptr var, ASTptr type);
    void accept(IVisitor &visitor);

    ASTptr var;
    ASTptr type;
};

class TypeSpecAST : public AST{
    public:
    TypeSpecAST(Token token);
    void accept(IVisitor &visitor);
};

class ConstAST : public AST{
    public:
    ConstAST(ASTptr constName, ASTptr constValue);
    void accept(IVisitor &visitor);

    ASTptr constName;
    ASTptr constValue;
};

class StringAST : public AST{
    public:
    StringAST(Token token);
    void accept(IVisitor &visitor);
};

class CallAST : public AST{
    public:
    CallAST(Token token, std::vector<ASTptr> params);
    void accept(IVisitor &visitor);

    std::vector<ASTptr> params;
};

class ifAST : public AST{
    public:
    ifAST(ASTptr condition, ASTptr body, ASTptr elseBody);
    void accept(IVisitor &visitor);

    ASTptr condition;
    ASTptr body;
    ASTptr elseBody;
};

class whileAST : public AST{
    public:
    whileAST(ASTptr condition, ASTptr body);
    void accept(IVisitor &visitor);

    ASTptr condition;
    ASTptr body;
};


#endif