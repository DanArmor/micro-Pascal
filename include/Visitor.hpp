#ifndef __INC_VISITOR_H
#define __INC_VISITOR_H

#include <fstream>
#include <string>

#include "Token.hpp"
#include "List.cpp"

#include "ASTclasses.hpp"

/// @brief Посетитель для вывода узлов АСД
//class ViewVisitor : public IVisitor{
//    public:
//    virtual void visit(BinOpAST &node){
//        std::cout << fmt::format("Token: {}  Type: {}\n", node.token.getStr(), node.token.getType());
//        node.left->accept(*this);
//        node.right->accept(*this);
//    }
//    virtual void visit(NumberAST &node){
//        std::cout << fmt::format("Token: {}  Type: {}\n", node.token.getStr(), node.token.getType());
//    }
//    virtual void visit(UnOpAST &node){
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
//    void visit(BinOpAST &node);
//
//    void visit(UnOpAST &node);
//
//    void visit(NumberAST &node);
//
//    void visit(CompoundAST &node);
//
//    void visit(AssignAST &node);
//
//    void visit(VarAST &node);
//
//    void visit(NoOpAST &node);
//};

/// @brief Посетителя для построения графического отображения АДС
class GraphvizVisitor : public IVisitor{
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

    std::vector<std::string> getData(void);

    private:
    std::vector<std::string> typesStrings;
};

/// @brief Посетитель-кодовый генератор
//class CodeGenVisitor : public IVisitor{
//    public:
//
//    explicit CodeGenVisitor(std::string filename);
//
//    void done(void);
//
//    void visit(BinOpAST &node);
//
//    void visit(UnOpAST &node);
//
//    void visit(NumberAST &node);
//
//    void visit(CompoundAST &node);
//
//    void visit(AssignAST &node);
//
//    void visit(VarAST &node);
//
//    void visit(NoOpAST &node);
//
//    void visit(ProgramAST &node);
//
//    void visit(BlockAST &node);
//
//    void visit(VarDeclAST &node);
//
//    void visit(TypeSpecAST &node);
//
//    void visit(ConstAST &node);
//
//    void visit(StringAST &node);
//
//    void visit(CallAST &node);
//
//    void visit(IfAST &node);
//
//    void visit(WhileAST &node);
//
//    void visit(ForAST &node);
//
//    void write(void);
//
//    private:
//    std::fstream file;
//};

/// @brief Посетитель подсветки синтаксиса
class HighlightAccurateVisitor : public IVisitor{
    public:

    explicit HighlightAccurateVisitor(List<Token> tokens);

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

    List<Token> getTokens(void);

    private:
    List<Token> tokens;
};

#endif