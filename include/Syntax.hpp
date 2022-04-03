#ifndef __INC_SYNTAX_H
#define __INC_SYNTAX_H

#include <vector>
#include <initializer_list>

#include "magic_enum.hpp"

#include "Token.hpp"
#include "AST.hpp"
#include "List.cpp"

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

    AST *program(void);

    AST *compountStatement(void);

    std::vector<ASTptr> statementList(void);

    ASTptr statement(void);

    ASTptr assignmentStatement(void);

    ASTptr variable(void);

    ASTptr emptyStatement(void);

    ASTptr block(void);

    std::vector<ASTptr> declarations(void);

    std::vector<ASTptr> varDeclaration(void);

    ASTptr typeSpec(void);

    AST *factor();

    AST *term(void);

    AST *expr(void);

    AST *simpleExpr(void);

    AST *parseTokens(void);

    AST *procCallStatement(void);

    AST *ifStatement(void);

    AST *whileStatement(void);

    void getNextToken(void);

    Token &getCurTok(void);

    Token &lookFoward(void);

    std::vector<ASTptr> constants(void);
    ASTptr constDeclaration(void);

    void eat(IToken::Type type);

    private:
    List<Token> tokens;
    std::size_t currentIndex = 0;
};  


#endif