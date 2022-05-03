#include <string>
#include <vector>
#include <gtest/gtest.h>

#include "List.cpp"
#include "Lexer.hpp"
#include "Syntax.hpp"
#include "SyntExp.hpp"
#include "AST.hpp"
#include "PascalTokens.hpp"
#include "Visitor.hpp"

/// @brief Тест семантического анализа Паскаля
TEST(SEMANTIC, PASCAL_1_THROW){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> tokens = lexer.analyzeFile("./build/tests/textM1.txt");

    SyntaxAnalyzer syntax(tokens);
    std::unique_ptr<AST> root(syntax.parseTokens());

    SemanticVisitor semantic;
    EXPECT_THROW(root->accept(semantic), TypeException);
}

/// @brief Тест семантического анализа Паскаля
TEST(SEMANTIC, PASCAL_2_THROW){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> tokens = lexer.analyzeFile("./build/tests/textM2.txt");

    SyntaxAnalyzer syntax(tokens);
    std::unique_ptr<AST> root(syntax.parseTokens());

    SemanticVisitor semantic;
    EXPECT_THROW(root->accept(semantic), TypeException);
}

/// @brief Тест семантического анализа Паскаля
TEST(SEMANTIC, PASCAL_3_THROW){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> tokens = lexer.analyzeFile("./build/tests/textM3.txt");

    SyntaxAnalyzer syntax(tokens);
    std::unique_ptr<AST> root(syntax.parseTokens());

    SemanticVisitor semantic;
    EXPECT_THROW(root->accept(semantic), TypeException);
}

/// @brief Тест семантического анализа Паскаля
TEST(SEMANTIC, PASCAL_4_THROW){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> tokens = lexer.analyzeFile("./build/tests/textM4.txt");

    SyntaxAnalyzer syntax(tokens);
    std::unique_ptr<AST> root(syntax.parseTokens());

    SemanticVisitor semantic;
    EXPECT_THROW(root->accept(semantic), SemanticException);
}

/// @brief Тест семантического анализа Паскаля
TEST(SEMANTIC, PASCAL_5_THROW){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> tokens = lexer.analyzeFile("./build/tests/textM5.txt");

    SyntaxAnalyzer syntax(tokens);
    std::unique_ptr<AST> root(syntax.parseTokens());

    SemanticVisitor semantic;
    EXPECT_THROW(root->accept(semantic), SemanticException);
}

/// @brief Тест семантического анализа Паскаля
TEST(SEMANTIC, PASCAL_6_THROW){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> tokens = lexer.analyzeFile("./build/tests/textM6.txt");

    SyntaxAnalyzer syntax(tokens);
    std::unique_ptr<AST> root(syntax.parseTokens());

    SemanticVisitor semantic;
    EXPECT_THROW(root->accept(semantic), SemanticException);
}


int main(void){
    testing::InitGoogleTest();

    return ::RUN_ALL_TESTS();
}