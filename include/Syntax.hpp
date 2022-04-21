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
    SyntaxAnalyzer(List<Token> const &tokens);

    AST *syntaxProgram(void);

    AST *syntaxCompoundSt(void);

    std::vector<AST*> syntaxStList(void);

    ASTptr syntaxSt(void);

    ASTptr syntaxAssignSt(void);

    ASTptr syntaxVariable(void);

    ASTptr syntaxEmptySt(void);

    ASTptr syntaxBlock(void);

    std::vector<AST*> syntaxVars(void);

    std::vector<AST*> syntaxVarDecl(void);

    ASTptr syntaxTypeSpec(void);

    AST *syntaxFactor();

    AST *syntaxTerm(void);

    AST *syntaxExpr(void);

    AST *syntaxSimpleExpr(void);

    AST *parseTokens(void);

    AST *syntaxCallSt(void);

    AST *syntaxIfSt(void);

    AST *syntaxWhileSt(void);

    void getNextToken(void);

    Token &getCurTok(void);

    Token &lookFoward(void);

    std::vector<AST*> syntaxConsts(void);
    ASTptr syntaxConstDecl(void);

    void eat(IToken::Type const type);
    void eatAdv(IToken::AdvType const type);

    private:
    List<Token> tokens;
    std::size_t currentIndex = 0;
};  


#endif