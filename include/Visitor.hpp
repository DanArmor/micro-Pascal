#ifndef __INC_VISITOR_H
#define __INC_VISITOR_H

#include <fstream>
#include <string>
#include <fmt/format.h>

#include "Token.hpp"
#include "AST.hpp"


#include "ASTclasses.hpp"

/// @brief Посетитель для вывода узлов АСД
//class ViewVisitor : public IVisitor{
//    public:
//    virtual void visit(BinOp &node){
//        std::cout << fmt::format("Token: {}  Type: {}\n", node.token.getStr(), node.token.getType());
//        node.left->accept(*this);
//        node.right->accept(*this);
//    }
//    virtual void visit(Number &node){
//        std::cout << fmt::format("Token: {}  Type: {}\n", node.token.getStr(), node.token.getType());
//    }
//    virtual void visit(UnOp &node){
//        std::cout << fmt::format("Token: {}  Type: {}\n", node.token.getStr(), node.token.getType());
//        node.down->accept(*this);
//    }
//};


/// @brief Класс-шаблон для получения посетителей, способных хранить в себе значения
template<typename VisitorImpl, typename VisitablePtr, typename ResultType>
class VisitorValue{

    public:
    /**
     * @brief Просит VisitablePtr принять VisitorImpl, возвращает значение VisitorImpl после завершения обхода
     * @param ptr Класс, способный принять посетителя 
     * @return ResultType Значение, сохранившееся в VisitorImpl после обхода
     */
    static ResultType getValue(VisitablePtr ptr){
        VisitorImpl vis;
        ptr->accept(vis);
        return vis.value;
    }

    /**
     * @brief Аналог return для посетителя, т.к. сигнатура функции предполагает возвращение void 
     * @param inValue Значение, которое сохранится внутри посетителя
     */
    void Return(ResultType inValue){
        value = inValue;
    }

    /// @brief Показывает хранимое внутри посетителя значение
    ResultType showValue(void){
        return value;
    }

    private:
    ResultType value;

};

/// @brief Посетитель для расчета по дереву выражения
//class CalcVisitor : public IVisitor, public VisitorValue<CalcVisitor, ASTptr, int>{
//
//    public:
//
//    CalcVisitor();
//
//    void visit(BinOp &node);
//
//    void visit(UnOp &node);
//
//    void visit(Number &node);
//
//    void visit(Compound &node);
//
//    void visit(Assign &node);
//
//    void visit(Var &node);
//
//    void visit(NoOp &node);
//};

/// @brief Посетителя для построения графического отображения АДС
class GraphvizVisitor : public IVisitor{
    public:

    GraphvizVisitor(std::string filename);

    void done(void);

    void visit(BinOp &node);

    void visit(UnOp &node);

    void visit(Number &node);

    void visit(Compound &node);

    void visit(Assign &node);

    void visit(Var &node);

    void visit(NoOp &node);

    void visit(ProgramAST &node);

    void visit(BlockAST &node);

    void visit(VarDeclaration &node);

    void visit(Type &node);

    void visit(ConstAST &node);

    void visit(StringAST &node);

    void visit(ProcedureCall &node);

    void visit(ifAST &node);

    void visit(whileAST &node);

    void write(void);

    private:
    std::fstream file;
    std::size_t nodeIndex = 0;
    std::vector<std::pair<std::string, std::string>> declarations;
    std::vector<std::pair<std::string, std::string>> links;
};

/// @brief Посетителя для вывода сведений о типах в прямом порядке по АДС
class TypeViewVisitor : public IVisitor{
    public:

    TypeViewVisitor();

    void visit(BinOp &node);

    void visit(UnOp &node);

    void visit(Number &node);

    void visit(Compound &node);

    void visit(Assign &node);

    void visit(Var &node);

    void visit(NoOp &node);

    void visit(ProgramAST &node);

    void visit(BlockAST &node);

    void visit(VarDeclaration &node);

    void visit(Type &node);

    void visit(ConstAST &node);

    void visit(StringAST &node);

    void visit(ProcedureCall &node);

    void visit(ifAST &node);

    void visit(whileAST &node);

    std::vector<std::string> getData(void);

    private:
    std::vector<std::string> typesStrings;
};

/// @brief Посетитель-кодовый генератор
class CodeGenVisitor : public IVisitor{
    public:

    CodeGenVisitor(std::string filename);

    void done(void);

    void visit(BinOp &node);

    void visit(UnOp &node);

    void visit(Number &node);

    void visit(Compound &node);

    void visit(Assign &node);

    void visit(Var &node);

    void visit(NoOp &node);

    void visit(ProgramAST &node);

    void visit(BlockAST &node);

    void visit(VarDeclaration &node);

    void visit(Type &node);

    void visit(ConstAST &node);

    void visit(StringAST &node);

    void visit(ProcedureCall &node);

    void visit(ifAST &node);

    void visit(whileAST &node);

    void write(void);

    private:
    std::fstream file;
};

#endif