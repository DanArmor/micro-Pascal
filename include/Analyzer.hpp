#ifndef _INC_ANALYZE_H
#define _INC_ANALYZE_H

/**
 * @file Analyzer.hpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Анализатор исходного кода
 * @version 1.0
 */

#include <string>

#include "ASTclasses.hpp"
#include "Lexer.hpp"
#include "List.hpp"
#include "Syntax.hpp"
#include "Token.hpp"
class Analyzer {
   public:
    Analyzer(std::unique_ptr<ISyntaxAnalyzer> syntax,
             List<TokenTemplate> templates);

    void addVisitor(std::unique_ptr<IVisitor> visitor);

    void highlightOutput();

    void analyzeFile(std::string inName);

   protected:
    Lexer lexer;
    std::vector<std::unique_ptr<IVisitor>> visitorsToRun;
    std::unique_ptr<ISyntaxAnalyzer> syntax;
};

#endif