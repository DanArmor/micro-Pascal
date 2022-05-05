#include <string>
#include <vector>
#include <gtest/gtest.h>

#include "List.hpp"
#include "Lexer.hpp"
#include "Syntax.hpp"
#include "SyntExp.hpp"
#include "AST.hpp"
#include "PascalRules.hpp"

void checkEqSyn(IAST *ptr, std::vector<std::string> const &checkData){
    TypeViewVisitor typeView;
    ptr->accept(typeView);
    std::vector<std::string> result = typeView.getData();
    EXPECT_EQ(result.size(), checkData.size());
    for(int i = 0; i < result.size(); i++)
        EXPECT_EQ(result[i], checkData[i]);
}


/// @brief Тест синтаксического анализа Паскаля, простой тест
TEST(SYNTAX, PASCAL_SIMPLE){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    std::vector<std::string> checkData = {
        "test",
        "BLOCK",
        "VARDECL",
        "x",
        "integer",
        "COMPOUND",
        ":=",
        "x",
        "1",
        "$"
    };
    List<Token> tokens = lexer.analyzeFile("./build/tests/textS1.txt");

    SyntaxAnalyzer syntax(tokens);
    std::unique_ptr<IAST> root(syntax.analyzeTokens());
    checkEqSyn(root.get(), checkData);
}

/// @brief Тест синтаксического анализа Паскаля, средний текст
TEST(SYNTAX, PASCAL_MEDIUM){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    std::vector<std::string> checkData = {
        "test",
        "BLOCK",
        "CONSTDECL",
        "a",
        "10",
        "CONSTDECL",
        "str",
        "'hello there'",

        "VARDECL",
        "x",
        "integer",

        "VARDECL",
        "y",
        "integer",

        "VARDECL",
        "z",
        "integer",

        "COMPOUND",
        "if",
        ":=",
        "z",
        "x",
        ">",
        "y",
        "x",
        ":=",
        "z",
        "y",
        "while",
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
    std::unique_ptr<IAST> root(syntax.analyzeTokens());
    checkEqSyn(root.get(), checkData);
}

/// @brief Тест синтаксического анализа Паскаля, средний текст с ошибкой
TEST(SYNTAX, PASCAL_MEDIUM_THROW){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> tokens = lexer.analyzeFile("./build/tests/textS3.txt");

    SyntaxAnalyzer syntax(tokens);
    EXPECT_THROW(syntax.analyzeTokens(), SyntaxException);
}

TEST(SYNTAX, PASCAL_THROW_1){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> tokens = lexer.analyzeFile("./build/tests/textS4.txt");

    SyntaxAnalyzer syntax(tokens);
    EXPECT_THROW(syntax.analyzeTokens(), SyntaxException);
}

TEST(SYNTAX, PASCAL_THROW_2){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> tokens = lexer.analyzeFile("./build/tests/textS5.txt");

    SyntaxAnalyzer syntax(tokens);
    EXPECT_THROW(syntax.analyzeTokens(), SyntaxException);
}

TEST(SYNTAX, PASCAL_THROW_3){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> tokens = lexer.analyzeFile("./build/tests/textS6.txt");

    SyntaxAnalyzer syntax(tokens);
    EXPECT_THROW(syntax.analyzeTokens(), SyntaxException);
}

/// @brief Тест синтаксического анализа Паскаля
TEST(SYNTAX, PASCAL_1){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    std::vector<std::string> checkData = {
        "test",
        "add",
        "VARDECL",
        "x",
        "integer",
        "VARDECL",
        "y",
        "integer",
        "integer",
        "BLOCK",
        "COMPOUND",
        "return",
        "+",
        "x",
        "y",
        "$",

        "BLOCK",
        "CONSTDECL",
        "a",
        "+",
        "1",
        "1",
        "CONSTDECL",
        "b",
        "a",

        "VARDECL",
        "x",
        "integer",

        "COMPOUND",
        ":=",
        "x",
        "mod",
        "5",
        "2",

        "$"
    };
    List<Token> tokens = lexer.analyzeFile("./build/tests/textS7.txt");

    SyntaxAnalyzer syntax(tokens);
    std::unique_ptr<IAST> root(syntax.analyzeTokens());
    checkEqSyn(root.get(), checkData);
}

/// @brief Тест синтаксического анализа Паскаля
TEST(SYNTAX, PASCAL_2){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    std::vector<std::string> checkData = {
        "test",
        "add",
        "VARDECL",
        "x",
        "integer",
        "VARDECL",
        "y",
        "integer",
        "integer",
        "BLOCK",
        "COMPOUND",
        "return",
        "+",
        "x",
        "y",
        "$",

        "BLOCK",

        "VARDECL",
        "x",
        "integer",

        "VARDECL",
        "i",
        "integer",

        "COMPOUND",
        ":=",
        "x",
        "1",

        "for",
        "to",
        ":=",
        "i",
        "1",
        "<=",
        "i",
        "5",
        "+",
        "i",
        "1",
        ":=",
        "x",
        "add",
        "x",
        "i",

        "$"
    };
    List<Token> tokens = lexer.analyzeFile("./build/tests/textS8.txt");

    SyntaxAnalyzer syntax(tokens);
    std::unique_ptr<IAST> root(syntax.analyzeTokens());
    checkEqSyn(root.get(), checkData);
}

int main(void){
    testing::InitGoogleTest();

    return ::RUN_ALL_TESTS();
}