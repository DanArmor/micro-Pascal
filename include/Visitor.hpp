#ifndef __INC_VISITOR_H
#define __INC_VISITOR_H

#include <fstream>
#include <string>

#include "Token.hpp"
#include "List.cpp"

#include "ASTclasses.hpp"
#include "SyntExp.hpp"
#include "sup.hpp"

/// @brief Посетителя для построения графического отображения АДС
class GraphvizVisitor : public IVisitor
{
public:
    explicit GraphvizVisitor(std::string filename);

    void done(void);

    void visit(BinOpAST &node);

    void visit(UnOpAST &node);

    void visit(NumberAST &node);

    void visit(CompoundAST &node);

    void visit(AssignAST &node);

    void visit(VarAST &node);

    void visit(NoOpAST &node);

    void visit(ProgramAST &node);

    void visit(BlockAST &node);

    void visit(VarDeclAST &node);

    void visit(TypeSpecAST &node);

    void visit(ConstAST &node);

    void visit(StringAST &node);

    void visit(CallAST &node);

    void visit(IfAST &node);

    void visit(WhileAST &node);

    void visit(ForAST &node);

    void visit(IterationAST &node);

    void visit(FunctionAST &node);

    void visit(ReturnAST &node);

    void visit(ArrSpecAST &node);

    void visit(SelectAST &node);

    void write(void);

private:
    std::fstream file;
    std::size_t nodeIndex = 0;
    std::vector<std::pair<std::string, std::string>> declarations;
    std::vector<std::pair<std::string, std::string>> links;
};

/// @brief Посетителя для вывода сведений о типах в прямом порядке по АДС
class TypeViewVisitor : public IVisitor
{
public:
    TypeViewVisitor();

    void visit(BinOpAST &node);

    void visit(UnOpAST &node);

    void visit(NumberAST &node);

    void visit(CompoundAST &node);

    void visit(AssignAST &node);

    void visit(VarAST &node);

    void visit(NoOpAST &node);

    void visit(ProgramAST &node);

    void visit(BlockAST &node);

    void visit(VarDeclAST &node);

    void visit(TypeSpecAST &node);

    void visit(ConstAST &node);

    void visit(StringAST &node);

    void visit(CallAST &node);

    void visit(IfAST &node);

    void visit(WhileAST &node);

    void visit(ForAST &node);

    void visit(IterationAST &node);

    void visit(FunctionAST &node);

    void visit(ReturnAST &node);

    void visit(ArrSpecAST &node);

    void visit(SelectAST &node);

    std::vector<std::string> getData(void);

private:
    std::vector<std::string> typesStrings;
};

class SemanticVisitor : public IVisitor
{
public:
    struct FunctionData
    {
        FunctionData(Token token);
        FunctionData(Token token, std::vector<std::string> params, std::string returnType);
        FunctionData();
        Token token;
        std::vector<std::string> params;
        std::string returnType;
    };

    struct VarData
    {
        VarData(Token token, bool isConst);
        VarData();
        Token token;
        bool isConst;
        std::string subType;
        std::string type;
    };

    SemanticVisitor();

    void visit(BinOpAST &node);

    void visit(UnOpAST &node);

    void visit(NumberAST &node);

    void visit(CompoundAST &node);

    void visit(AssignAST &node);

    void visit(VarAST &node);

    void visit(NoOpAST &node);

    void visit(ProgramAST &node);

    void visit(BlockAST &node);

    void visit(VarDeclAST &node);

    void visit(TypeSpecAST &node);

    void visit(ConstAST &node);

    void visit(StringAST &node);

    void visit(CallAST &node);

    void visit(IfAST &node);

    void visit(WhileAST &node);

    void visit(ForAST &node);

    void visit(IterationAST &node);

    void visit(FunctionAST &node);

    void visit(ReturnAST &node);

    void visit(ArrSpecAST &node);

    void visit(SelectAST &node);

    void addProgName(std::string name) {
        programName = name;
    }

    void addVar(Token token) {
        if (vars.count(token.getStr()) != 0)
            throw SemanticException(token, "Повторное объявление! ");
        vars[token.getStr()] = VarData(token, false);
    }

    void addConst(Token token) {
        if (consts.count(token.getStr()) != 0)
            throw SemanticException(token, "Повторное объявление! ");
        consts[token.getStr()] = VarData(token, true);
    }

    VarData &getVar(Token name) {
        checkVar(name);
        return vars[name.getStr()];
    }

    VarData &getConst(Token name) {
        checkConst(name);
        return consts[name.getStr()];
    }

    VarData &getDefined(Token name) {
        checkDefined(name);
        if (checkVar(name))
            return vars[name.getStr()];
        else
            return consts[name.getStr()];
    }

    void checkDefined(Token name) {
        if (!checkVar(name) && !checkConst(name))
            throw SyntaxException(name, "Использование до объявления! ");
    }

    void addFunc(Token token) {
        if (functions.count(token.getStr()) != 0)
            throw SyntaxException(token, "Повторное объявление подпрограммы! ");
        functions[token.getStr()] = FunctionData(token);
    }

    void prebuildFunction(std::string name, std::vector<std::string> params, std::string returnType){
        functions[name] = FunctionData(Token(name, IToken::ID, IToken::FUNCTION_NAME), params, returnType);
    }

    FunctionData &getFunc(Token name) {
        checkFunc(name);
        return functions[name.getStr()];
    }

    bool checkVar(Token name) {
        return vars.count(name.getStr()) != 0;
    }

    bool checkConst(Token name) {
        return consts.count(name.getStr()) != 0;
    }

    void checkFunc(Token token) {
        if (functions.count(token.getStr()) == 0)
            throw SyntaxException(token, "Использование до объявления! ");
    }

    bool compareTypes(std::string A, std::string B, bool strict = false) {
        if (strict)
        {
            return A == B;
        }
        else
        {
            if (isIn(A, {std::string("integer"), std::string("real")}) && isIn(B, {std::string("integer"), std::string("real")}))
            {
                return true;
            }
            return A == B;
        }
    }

    std::string getSubType(ArrSpecAST *node);

    void clearBlock(void) {
        vars.clear();
        consts.clear();
    }

    std::string getValue(AST *ptr) {
        auto vis = *this;
        ptr->accept(vis);
        return vis.value;
    }

    /**
     * @brief Аналог return для посетителя, т.к. сигнатура функции предполагает возвращение void
     * @param inValue Значение, которое сохранится внутри посетителя
     */
    void Return(std::string inValue) {
        value = inValue;
    }

    /// @brief Показывает хранимое внутри посетителя значение
    std::string showValue(void) {
        return value;
    }

private:
    std::string value;
    std::string programName;
    std::string currCheckFunc;
    std::map<std::string, VarData> vars;
    std::map<std::string, VarData> consts;
    std::map<std::string, FunctionData> functions;
};

#endif