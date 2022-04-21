#ifndef __INC_AST_H
#define __INC_AST_H

#include <vector>
#include <memory>

#include "Token.hpp"

#include "Visitor.hpp"
#include "ASTclasses.hpp"

/// @brief Узел, содержащий бинарную операцию
class BinOpAST : public AST{
    public:

    BinOpAST(Token token, AST *left, AST *right);
    ~BinOpAST() = default;

    void accept(IVisitor &visitor);

    std::unique_ptr<AST> left;
    std::unique_ptr<AST> right;
};

/// @brief Узел, содержащий унарную операцию
class UnOpAST : public AST{
    public:

    UnOpAST(Token token, AST *down);
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

    void addChild(AST *child);

    void accept(IVisitor &visitor);

    std::vector<std::unique_ptr<AST>> children;
};

/// @brief Узел, содержащий бинарную операцию
class AssignAST : public AST{
    public:

    AssignAST(Token token, AST *var, AST *value);
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

    BlockAST(Token token, std::vector<AST*> consts, std::vector<AST*> declarations, ASTptr compound);
    ~BlockAST() = default;
    void accept(IVisitor &visitor);

    std::vector<std::unique_ptr<AST>> consts;
    std::vector<std::unique_ptr<AST>> declarations;
    std::unique_ptr<AST> compound;
};

class VarDeclAST : public AST{
    public:

    VarDeclAST(Token token, AST *var, AST *type);
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

class ConstAST : public AST{
    public:

    ConstAST(Token token, ASTptr constName, ASTptr constValue);
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

    CallAST(Token token, std::vector<AST*> params);
    ~CallAST() = default;
    void accept(IVisitor &visitor);

    std::vector<std::unique_ptr<AST>> params;
};

class IfAST : public AST{
    public:

    IfAST(Token token, ASTptr condition, ASTptr body, ASTptr elseBody);
    ~IfAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<AST> condition;
    std::unique_ptr<AST> body;
    std::unique_ptr<AST> elseBody;
};

class WhileAST : public AST{
    public:

    WhileAST(Token token, ASTptr condition, ASTptr body);
    ~WhileAST() = default;
    void accept(IVisitor &visitor);

    std::unique_ptr<AST> condition;
    std::unique_ptr<AST> body;
};


#endif