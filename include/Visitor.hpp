#ifndef __INC_VISITOR_H
#define __INC_VISITOR_H

/**
 * @file Visitor.hpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Посетители для обхода ДАС
 * @version 1.0
 */

#include <fstream>
#include <memory>
#include <string>

#include "ASTclasses.hpp"
#include "List.hpp"
#include "Token.hpp"

/// @brief Посетителя для построения графического отображения ДАС
class GraphvizVisitor : public IVisitor {
   public:
    GraphvizVisitor(std::string filename);

    void visit(ProgramAST &node);

    void visit(FunctionAST &node);

    void visit(BlockAST &node);

    void visit(VarDeclAST &node);

    void visit(TypeSpecAST &node);

    void visit(ArrSpecAST &node);

    void visit(ConstAST &node);

    void visit(CompoundAST &node);

    void visit(NumberAST &node);

    void visit(StringAST &node);

    void visit(BinOpAST &node);

    void visit(UnOpAST &node);

    void visit(NoOpAST &node);

    void visit(AssignAST &node);

    void visit(VarAST &node);

    void visit(SelectAST &node);

    void visit(CallAST &node);

    void visit(ReturnAST &node);

    void visit(IfAST &node);

    void visit(WhileAST &node);

    void visit(ForAST &node);

    void visit(IterationAST &node);

    void done(void);
    void write(void);

   private:
    void addDef(std::string const &str);
    void connectToNode(std::size_t index, IAST *ptr);
    std::fstream file;
    std::string outName;
    std::size_t nodeIndex = 0;
    std::vector<std::pair<std::string, std::string>> declarations;
    std::vector<std::pair<std::string, std::string>> links;
};

/// @brief Посетителя для вывода сведений о типах в прямом порядке по ДАС
class TypeViewVisitor : public IVisitor {
   public:
    TypeViewVisitor();

    void visit(ProgramAST &node);

    void visit(FunctionAST &node);

    void visit(BlockAST &node);

    void visit(VarDeclAST &node);

    void visit(TypeSpecAST &node);

    void visit(ArrSpecAST &node);

    void visit(ConstAST &node);

    void visit(CompoundAST &node);

    void visit(NumberAST &node);

    void visit(StringAST &node);

    void visit(BinOpAST &node);

    void visit(UnOpAST &node);

    void visit(NoOpAST &node);

    void visit(AssignAST &node);

    void visit(VarAST &node);

    void visit(SelectAST &node);

    void visit(CallAST &node);

    void visit(ReturnAST &node);

    void visit(IfAST &node);

    void visit(WhileAST &node);

    void visit(ForAST &node);

    void visit(IterationAST &node);

    std::vector<std::string> getData(void);

   private:
    std::vector<std::string> typesStrings;
};

/// @brief Посетитель-семантический анализатор
class SemanticVisitor : public IVisitor {
   public:
    ///@brief Структура для хранения данных о функции
    struct FunctionData {
        explicit FunctionData(Token token);
        FunctionData(Token token, std::vector<std::string> params,
                     std::string returnType);
        FunctionData();
        Token token;
        std::vector<std::string> params;
        std::string returnType;
    };

    ///@brief Структура для хранения данных об именованном значении - переменной
    ///или константе
    struct VarData {
        VarData(Token token, IAST *typePtr);
        VarData(Token token, bool isConst);
        VarData();
        IAST *typePtr = nullptr;
        Token token;
        bool isConst;
        std::string type;
    };

    SemanticVisitor();

    void visit(ProgramAST &node);

    void visit(FunctionAST &node);

    void visit(BlockAST &node);

    void visit(VarDeclAST &node);

    void visit(TypeSpecAST &node);

    void visit(ArrSpecAST &node);

    void visit(ConstAST &node);

    void visit(CompoundAST &node);

    void visit(NumberAST &node);

    void visit(StringAST &node);

    void visit(BinOpAST &node);

    void visit(UnOpAST &node);

    void visit(NoOpAST &node);

    void visit(AssignAST &node);

    void visit(VarAST &node);

    void visit(SelectAST &node);

    void visit(CallAST &node);

    void visit(ReturnAST &node);

    void visit(IfAST &node);

    void visit(WhileAST &node);

    void visit(ForAST &node);

    void visit(IterationAST &node);

    void addProgName(std::string name);

    void addVar(Token token, IAST *ptr);
    void addVar(Token token);
    VarData &getVar(Token name);

    void addConst(Token token);
    VarData &getConst(Token name);

    bool checkVar(Token name);
    bool checkConst(Token name);
    void checkDefined(Token name);

    VarData &getDefined(Token name);

    void addFunc(Token token);
    void checkFunc(Token token);
    FunctionData &getFunc(Token name);

    /// @brief Добавление стандартной функции
    void prebuildFunction(Token tok, std::vector<std::string> params,
                          std::string returnType);
    /// @brief Добавление стандартных функций
    void prebuildFunctions(List<FunctionData> funcs);

    /// @brief Сравнение типов по правилам Паскаля. Если strict == true, то
    /// имена типов должны абсолютно совпадать
    bool compareTypes(std::string A, std::string B, bool strict = false);

    /// @brief Очищение списка переменных и констант
    void clearBlock(void);

    /// @brief Создает копию посетителя, которая посещает ptr и возвращает
    /// результат
    std::string getValue(IAST *ptr);

    /**
     * @brief Аналог return для посетителя
     * @param inValue Значение, которое сохранится внутри посетителя
     */
    void Return(std::string inValue);

   private:
    std::size_t typeIndex = 0;
    bool isConst = false;
    std::string value;
    std::string programName;
    std::string currCheckFunc;
    std::shared_ptr<std::map<std::string, VarData>> vars;
    std::shared_ptr<std::map<std::string, VarData>> consts;
    std::shared_ptr<std::map<std::string, FunctionData>> functions;
};

#endif