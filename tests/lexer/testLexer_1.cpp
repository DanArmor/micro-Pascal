#include <vector>
#include <gtest/gtest.h>

#include "List.cpp"
#include "Lexer.hpp"
#include "PascalTokens.hpp"
#include "lexerTestRules.hpp"

/**
 * @file testLexer_1.cpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Тесты для лексера
 * @version 0.5
 * @date 2022-03-31
 */

/// @brief Тест разбора простых математических выражений
TEST(PARSE, MATH){
    Lexer lexer;
    lexer.setTemplates(LexerTestRules::getMathRules());
    List<Token> checkData{std::vector<Token>{
        {"1", IToken::INTEGER_CONST},
        {"+", IToken::PLUS},
        {"1", IToken::INTEGER_CONST},
        {"=", IToken::EQ},
        {"2", IToken::INTEGER_CONST},
        {"3", IToken::INTEGER_CONST},
        {"+", IToken::PLUS},
        {"3", IToken::INTEGER_CONST},
        {"=", IToken::EQ},
        {"6", IToken::INTEGER_CONST},
        {"1984", IToken::INTEGER_CONST},
        {"1", IToken::INTEGER_CONST},
        {"*", IToken::MUL},
        {"9", IToken::INTEGER_CONST},
        {"/", IToken::FLOAT_DIV},
        {"(", IToken::LPAREN},
        {"1", IToken::INTEGER_CONST},
        {"-", IToken::MINUS},
        {"2", IToken::INTEGER_CONST},
        {")", IToken::RPAREN},
        {"$", IToken::ENDOFSTREAM},
    }};
    List<Token> result = lexer.analyzeFile("./build/tests/text1.txt");
    EXPECT_EQ(result.size(), checkData.size());
    for(int i = 0; i < result.size(); i++){
        EXPECT_EQ(result[i].getStr(), checkData[i].getStr());
        EXPECT_EQ(result[i].getType(), checkData[i].getType());
    }
}

/// @brief Тест разбора простейшей программы на Pascal
TEST(PARSE, PASCAL_SIMPLE){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> checkData{std::vector<Token>{
        {"PROGRAM", IToken::PROGRAM},
        {"test", IToken::ID},
        {";", IToken::SEMI},
        {"BEGIN", IToken::BEGIN},
        {"END", IToken::END},
        {".", IToken::DOT},
        {"$", IToken::ENDOFSTREAM},
    }};
    List<Token> result = lexer.analyzeFile("./build/tests/text2.txt");
    EXPECT_EQ(result.size(), checkData.size());
    for(int i = 0; i < result.size(); i++){
        EXPECT_EQ(result[i].getStr(), checkData[i].getStr());
        EXPECT_EQ(result[i].getType(), checkData[i].getType());
    }
}

/// @brief Тест разбора средней программы на Pascal
TEST(PARSE, PASCAL_MEDIUM){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> checkData{std::vector<Token>{
        {"PROGRAM", IToken::PROGRAM},
        {"test", IToken::ID},
        {";", IToken::SEMI},

        {"CONST", IToken::CONST},
        {"x", IToken::ID},
        {"=", IToken::EQ},
        {"10", IToken::INTEGER_CONST},
        {";", IToken::SEMI},

        {"VAR", IToken::VAR},
        {"a", IToken::ID},
        {",", IToken::COMMA},
        {"b", IToken::ID},
        {",", IToken::COMMA},
        {"c", IToken::ID},
        {":", IToken::COLON},
        {"INTEGER", IToken::INTEGER},
        {";", IToken::SEMI},

        {"BEGIN", IToken::BEGIN},

        {"a", IToken::ID},
        {":=", IToken::ASSIGN},
        {"0", IToken::INTEGER_CONST},
        {";", IToken::SEMI},
        {"b", IToken::ID},
        {":=", IToken::ASSIGN},
        {"0", IToken::INTEGER_CONST},
        {";", IToken::SEMI},
        {"c", IToken::ID},
        {":=", IToken::ASSIGN},
        {"x", IToken::ID},
        {";", IToken::SEMI},
        {"writeln", IToken::ID},
        {"(", IToken::LPAREN},
        {"'TEST STRING;1+1<>!=@@@###!!'", IToken::STRING_CONST},
        {")", IToken::RPAREN},
        {";", IToken::SEMI},

        {"END", IToken::END},
        {".", IToken::DOT},
        {"$", IToken::ENDOFSTREAM},
    }};
    List<Token> result = lexer.analyzeFile("./build/tests/text3.txt");
    EXPECT_EQ(result.size(), checkData.size());
    for(int i = 0; i < result.size(); i++){
        EXPECT_EQ(result[i].getStr(), checkData[i].getStr());
        EXPECT_EQ(result[i].getType(), checkData[i].getType());
    }
}

/// @brief Тест разбора сложной программы на Pascal
TEST(PARSE, PASCAL_HARD){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> checkData{std::vector<Token>{
        {"PROGRAM", IToken::PROGRAM},
        {"test", IToken::ID},
        {";", IToken::SEMI},

        {"CONST", IToken::CONST},
        {"x", IToken::ID},
        {"=", IToken::EQ},
        {"10", IToken::INTEGER_CONST},
        {";", IToken::SEMI},

        {"VAR", IToken::VAR},
        {"a", IToken::ID},
        {",", IToken::COMMA},
        {"b", IToken::ID},
        {",", IToken::COMMA},
        {"c", IToken::ID},
        {":", IToken::COLON},
        {"INTEGER", IToken::INTEGER},
        {";", IToken::SEMI},

        {"BEGIN", IToken::BEGIN},

        {"IF", IToken::IF},
        {"a", IToken::ID},
        {">", IToken::MORE},
        {"b", IToken::ID},
        {":", IToken::COLON},
        {"throw", IToken::ID},
        {"'hi'", IToken::STRING_CONST},
        {";", IToken::SEMI},

        {"writeln", IToken::ID},
        {"(", IToken::LPAREN},
        {"'TEST STRING;1+1<>!=@@@###!!'", IToken::STRING_CONST},
        {")", IToken::RPAREN},
        {";", IToken::SEMI},

        {"END", IToken::END},
        {".", IToken::DOT},
        {"$", IToken::ENDOFSTREAM},
    }};
    List<Token> result = lexer.analyzeFile("./build/tests/text4.txt");
    EXPECT_EQ(result.size(), checkData.size());
    for(int i = 0; i < result.size(); i++){
        EXPECT_EQ(result[i].getStr(), checkData[i].getStr());
        EXPECT_EQ(result[i].getType(), checkData[i].getType());
    }
}

/// @brief Тест разбора сложной программы на Pascal
TEST(PARSE, PASCAL_HARD_THROW){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    EXPECT_THROW(lexer.analyzeFile("./build/tests/text5.txt"), std::invalid_argument);
}

int main(void){
    testing::InitGoogleTest();

    return ::RUN_ALL_TESTS();
}