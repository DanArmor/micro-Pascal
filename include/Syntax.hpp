#ifndef __INC_SYNTAX_H
#define __INC_SYNTAX_H

#include <vector>

#include "magic_enum.hpp"

#include "List.hpp"
#include "Token.hpp"
#include "ASTclasses.hpp"

#include "sup.hpp"

/// @brief Класс синтаксического анализатора
class SyntaxAnalyzer{

    public:
    explicit SyntaxAnalyzer(List<Token> const &tokens);

    /// @brief Синтаксический анализ токенов, переданных анализатору.
    /// @details Обратите внимание, что синт. анализатор изменяет дополнительные типы некоторых токенов внутри собственного списка в процессе анализа   
    std::unique_ptr<AST> parseTokens(void);

    std::unique_ptr<AST> syntaxProgram(void);

    std::unique_ptr<AST> syntaxFuncDef(void);

    std::unique_ptr<AST> syntaxBlock(void);

    std::vector<std::unique_ptr<AST>> syntaxConsts(void);
    std::unique_ptr<AST> syntaxConstDecl(void);

    std::vector<std::unique_ptr<AST>> syntaxVars(void);
    std::vector<std::unique_ptr<AST>> syntaxVarDecl(void);
    std::unique_ptr<AST> syntaxTypeSpec(void);


    std::unique_ptr<AST> syntaxCompoundSt(void);
    std::vector<std::unique_ptr<AST>> syntaxStList(void);
    std::unique_ptr<AST> syntaxSt(void);

    std::unique_ptr<AST> syntaxEmptySt(void);
    std::unique_ptr<AST> syntaxAssignSt(void);
    std::unique_ptr<AST> syntaxIfSt(void);
    std::unique_ptr<AST> syntaxWhileSt(void);
    std::unique_ptr<AST> syntaxForSt(void);
    std::unique_ptr<AST> syntaxCallSt(void);
    std::unique_ptr<AST> syntaxReturnSt(void);


    std::unique_ptr<AST> syntaxVariable(void);
    std::unique_ptr<AST> syntaxSelect(void);

    std::unique_ptr<AST> syntaxExpr(void);
    std::unique_ptr<AST> syntaxSimpleExpr(void);
    std::unique_ptr<AST> syntaxTerm(void);
    std::unique_ptr<AST> syntaxFactor();

    std::unique_ptr<AST> syntaxIter(void);

    /// @brief Возвращает список токенов анализатора
    List<Token> getTokens(void);

    void getNextToken(void);

    Token &getCurTok(void);

    Token &lookFoward(void);

    /// @brief Пропускает текущий токен, если он указанного типа. Иначе выбрасывается исключение
    void eat(IToken::Type const type);
    /// @brief Пропускает текущий токен, если он указанного типа. Иначе выбрасывается исключение
    void eatAdv(IToken::AdvType const type);

    private:
    std::unique_ptr<AST> createCopyOfType(AST *ptr);

    List<Token> tokens;
    std::size_t currentIndex = 0;
};  


#endif