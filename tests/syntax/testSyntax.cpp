#include <string>
#include <vector>
#include <gtest/gtest.h>

#include "List.cpp"
#include "Lexer.hpp"
#include "Syntax.hpp"
#include "AST.hpp"
#include "PascalTokens.hpp"


/// @brief Тест синтаксического анализа Паскаля, простой тест
TEST(SYNTAX, PASCAL_SIMPLE){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    std::vector<std::string> checkData = {
        "PROGRAM:\ntest",
        "BLOCK",
        "VARDECL",
        "x",
        "INTEGER",
        "COMPOUND",
        ":=",
        "x",
        "1",
        "$"
    };
    List<Token> tokens = lexer.analyzeFile("./build/tests/textS1.txt");

    SyntaxAnalyzer syntax(tokens);
    ASTptr root = syntax.parseTokens();

    TypeViewVisitor typeView;
    root->accept(typeView);
    std::vector<std::string> result = typeView.getData();
    EXPECT_EQ(result.size(), checkData.size());
    for(int i = 0; i < result.size(); i++)
        EXPECT_EQ(result[i], checkData[i]);
}

/// @brief Тест синтаксического анализа Паскаля, средний текст
TEST(SYNTAX, PASCAL_MEDIUM){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    std::vector<std::string> checkData = {
        "PROGRAM:\ntest",
        "BLOCK",
        "CONSTDECL",
        "a",
        "10",
        "CONSTDECL",
        "str",
        "'hello there'",

        "VARDECL",
        "x",
        "INTEGER",

        "VARDECL",
        "y",
        "INTEGER",

        "VARDECL",
        "z",
        "INTEGER",

        "COMPOUND",
        "IF",
        ":=",
        "z",
        "x",
        ">",
        "y",
        "x",
        ":=",
        "z",
        "y",
        "WHILE",
        ":=",
        "z",
        "+",
        "z",
        "1",
        "<",
        "z",
        "y",
        "$"
    };
    List<Token> tokens = lexer.analyzeFile("./build/tests/textS2.txt");

    SyntaxAnalyzer syntax(tokens);
    ASTptr root = syntax.parseTokens();

    TypeViewVisitor typeView;
    root->accept(typeView);
    std::vector<std::string> result = typeView.getData();
    EXPECT_EQ(result.size(), checkData.size());
    for(int i = 0; i < result.size(); i++)
        EXPECT_EQ(result[i], checkData[i]);
}

/// @brief Тест синтаксического анализа Паскаля, средний текст с ошибкой
TEST(SYNTAX, PASCAL_MEDIUM_THROW){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> tokens = lexer.analyzeFile("./build/tests/textS3.txt");

    SyntaxAnalyzer syntax(tokens);
    EXPECT_THROW(syntax.parseTokens(), std::invalid_argument);
}

int main(void){
    testing::InitGoogleTest();

    return ::RUN_ALL_TESTS();
}