#ifndef __INC_SYNTAX_H
#define __INC_SYNTAX_H

#include <vector>
#include <initializer_list>

#include "magic_enum.hpp"

#include "List.cpp"
#include "Token.hpp"
#include "ASTclasses.hpp"

/**
 * @brief Возвращает истину, если value находится среди значений cont
 * @tparam T тип значений
 * @param value значение для поиска
 * @param cont среди каких значений искать
 * @return true Если value имеется среди cont
 * @return false Если value отсутствует среди cont
 */
template<typename T>
bool isIn(T const &value, std::initializer_list<T> const &cont){
    return std::find(cont.begin(), cont.end(), value) != cont.end();
}

/// @brief Класс синтаксического анализатора
class SyntaxAnalyzer{

    public:
    explicit SyntaxAnalyzer(List<Token> const &tokens);

    std::unique_ptr<AST> syntaxProgram(void);

    std::unique_ptr<AST> syntaxCompoundSt(void);

    std::vector<std::unique_ptr<AST>> syntaxStList(void);

    std::unique_ptr<AST> syntaxSt(void);

    std::unique_ptr<AST> syntaxAssignSt(void);

    std::unique_ptr<AST> syntaxVariable(void);

    std::unique_ptr<AST> syntaxEmptySt(void);

    std::unique_ptr<AST> syntaxBlock(void);

    std::vector<std::unique_ptr<AST>> syntaxVars(void);

    std::vector<std::unique_ptr<AST>> syntaxVarDecl(void);

    std::unique_ptr<AST> syntaxTypeSpec(void);

    std::unique_ptr<AST> syntaxFactor();

    std::unique_ptr<AST> syntaxTerm(void);

    std::unique_ptr<AST> syntaxExpr(void);

    std::unique_ptr<AST> syntaxSimpleExpr(void);

    std::unique_ptr<AST> parseTokens(void);

    std::unique_ptr<AST> syntaxCallSt(void);

    std::unique_ptr<AST> syntaxIfSt(void);

    std::unique_ptr<AST> syntaxWhileSt(void);

    std::unique_ptr<AST> syntaxForSt(void);

    std::unique_ptr<AST> syntaxIterSt(void);

    void getNextToken(void);

    Token &getCurTok(void);

    Token &lookFoward(void);

    std::vector<std::unique_ptr<AST>> syntaxConsts(void);
    std::unique_ptr<AST> syntaxConstDecl(void);

    void eat(IToken::Type const type);
    void eatAdv(IToken::AdvType const type);

    private:
    List<Token> tokens;
    std::size_t currentIndex = 0;
};  


#endif