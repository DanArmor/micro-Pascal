#ifndef __INC_PASCALSYNTAX_H
#define __INC_PASCALSYNTAX_H

/**
 * @file Syntax.hpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Синтаксический анализатор
 * @version 1.0
 */

#include <vector>

#include "ASTclasses.hpp"
#include "Syntax.hpp"
#include "List.hpp"
#include "Token.hpp"
#include "magic_enum.hpp"

/// @brief Класс синтаксического анализатора
class PascalSyntaxAnalyzer : public ISyntaxAnalyzer {
   public:
    PascalSyntaxAnalyzer();
    explicit PascalSyntaxAnalyzer(List<Token> const &tokens);

    /// @brief Синтаксический анализ токенов, переданных анализатору.
    /// @details Обратите внимание, что синт. анализатор изменяет дополнительные
    /// типы некоторых токенов внутри собственного списка в процессе анализа
    std::unique_ptr<IAST> analyzeTokens(void);

    std::unique_ptr<IAST> syntaxProgram(void);

    std::unique_ptr<IAST> syntaxFuncDef(void);

    std::unique_ptr<IAST> syntaxBlock(void);

    std::vector<std::unique_ptr<IAST>> syntaxConsts(void);
    std::unique_ptr<IAST> syntaxConstDecl(void);

    std::vector<std::unique_ptr<IAST>> syntaxVars(void);
    std::vector<std::unique_ptr<IAST>> syntaxVarDecl(void);
    std::unique_ptr<IAST> syntaxTypeSpec(void);

    std::unique_ptr<IAST> syntaxCompoundSt(void);
    std::vector<std::unique_ptr<IAST>> syntaxStList(void);
    std::unique_ptr<IAST> syntaxSt(void);

    std::unique_ptr<IAST> syntaxEmptySt(void);
    std::unique_ptr<IAST> syntaxAssignSt(void);
    std::unique_ptr<IAST> syntaxIfSt(void);
    std::unique_ptr<IAST> syntaxWhileSt(void);
    std::unique_ptr<IAST> syntaxForSt(void);
    std::unique_ptr<IAST> syntaxCallSt(void);
    std::unique_ptr<IAST> syntaxReturnSt(void);

    std::unique_ptr<IAST> syntaxVariable(void);
    std::unique_ptr<IAST> syntaxSelect(void);

    std::unique_ptr<IAST> syntaxExpr(void);
    std::unique_ptr<IAST> syntaxSimpleExpr(void);
    std::unique_ptr<IAST> syntaxTerm(void);
    std::unique_ptr<IAST> syntaxFactor(void);

    std::unique_ptr<IAST> syntaxIter(void);

    /// @brief Устанавливает список токенов анализатора
    void setTokens(List<Token> const &newTokens);
    /// @brief Возвращает список токенов анализатора
    List<Token> getTokens(void);

    void getNextToken(void);

    Token &getCurTok(void);

    Token &lookFoward(void);

    /// @brief Пропускает текущий токен, если он указанного типа. Иначе
    /// выбрасывается исключение
    void eat(IToken::Type const type);
    /// @brief Пропускает текущий токен, если он указанного типа. Иначе
    /// выбрасывается исключение
    void eatAdv(IToken::AdvType const type);

   private:
    List<Token> tokens;
    std::size_t currentIndex = 0;
};

#endif