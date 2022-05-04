#include <vector>
#include <gtest/gtest.h>

#include "List.hpp"
#include "Lexer.hpp"
#include "PascalRules.hpp"
#include "lexerTestRules.hpp"
#include "SyntExp.hpp"

/**
 * @file testLexer_1.cpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Тесты для лексера
 * @version 0.5
 * @date 2022-03-31
 */

void checkEqualLex(List<Token> checkData, Lexer lexer, std::string const &fname){
    List<Token> result = lexer.analyzeFile(fname);
    EXPECT_EQ(result.size(), checkData.size());
    for(int i = 0; i < result.size(); i++){
        EXPECT_EQ(result[i].getStr(), checkData[i].getStr());
        EXPECT_EQ(result[i].getType(), checkData[i].getType());
    }

}

/// @brief Тест разбора простых математических выражений
TEST(PARSE, MATH){
    Lexer lexer;
    lexer.setTemplates(LexerTestRules::getMathRules());
    List<Token> checkData{
        {"1", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {"+", IToken::PLUS, IToken::OPERATOR},
        {"1", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {"=", IToken::EQ, IToken::OPERATOR},
        {"2", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {"3", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {"+", IToken::PLUS, IToken::OPERATOR},
        {"3", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {"=", IToken::EQ, IToken::OPERATOR},
        {"6", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {"1984", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {"1", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {"*", IToken::MUL, IToken::OPERATOR},
        {"9", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {"/", IToken::FLOAT_DIV, IToken::OPERATOR},
        {"(", IToken::LPAREN, IToken::OPERATOR},
        {"1", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {"-", IToken::MINUS, IToken::OPERATOR},
        {"2", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {")", IToken::RPAREN, IToken::OPERATOR},
        {"$", IToken::ENDOFSTREAM, IToken::UNKNOWN},
    };
    checkEqualLex(checkData, lexer, "./build/tests/text1.txt");
}

/// @brief Тест разбора простейшей программы на Pascal
TEST(PARSE, PASCAL_SIMPLE){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> checkData{
        {"program", IToken::PROGRAM, IToken::KEYWORD},
        {"test", IToken::ID, IToken::PROGRAM_NAME},
        {";", IToken::SEMI, IToken::OPERATOR},
        {"begin", IToken::BEGIN, IToken::KEYWORD},
        {"end", IToken::END, IToken::KEYWORD},
        {".", IToken::DOT, IToken::OPERATOR},
        {"$", IToken::ENDOFSTREAM, IToken::UNKNOWN},
    };
    checkEqualLex(checkData, lexer, "./build/tests/text2.txt");
}

/// @brief Тест разбора средней программы на Pascal
TEST(PARSE, PASCAL_MEDIUM){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> checkData{
        {"program", IToken::PROGRAM, IToken::KEYWORD},
        {"test", IToken::ID, IToken::PROGRAM_NAME},
        {";", IToken::SEMI, IToken::OPERATOR},

        {"const", IToken::CONST, IToken::SOME_CONST},
        {"x", IToken::ID, IToken::VAR_NAME},
        {"=", IToken::EQ, IToken::OPERATOR},
        {"10", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {";", IToken::SEMI, IToken::OPERATOR},

        {"var", IToken::VAR, IToken::KEYWORD},
        {"a", IToken::ID, IToken::VAR_NAME},
        {",", IToken::COMMA, IToken::OPERATOR},
        {"b", IToken::ID, IToken::VAR_NAME},
        {",", IToken::COMMA, IToken::OPERATOR},
        {"c", IToken::ID, IToken::VAR_NAME},
        {":", IToken::COLON, IToken::OPERATOR},
        {"integer", IToken::INTEGER, IToken::TYPE_SPEC},
        {";", IToken::SEMI, IToken::OPERATOR},

        {"begin", IToken::BEGIN, IToken::KEYWORD},

        {"a", IToken::ID, IToken::VAR_NAME},
        {":=", IToken::ASSIGN, IToken::OPERATOR},
        {"0", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {";", IToken::SEMI, IToken::OPERATOR},
        {"b", IToken::ID, IToken::VAR_NAME},
        {":=", IToken::ASSIGN, IToken::OPERATOR},
        {"0", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {";", IToken::SEMI, IToken::OPERATOR},
        {"c", IToken::ID, IToken::VAR_NAME},
        {":=", IToken::ASSIGN, IToken::OPERATOR},
        {"x", IToken::ID, IToken::VAR_NAME},
        {";", IToken::SEMI, IToken::OPERATOR},
        {"writeln", IToken::ID, IToken::FUNCTION_NAME},
        {"(", IToken::LPAREN, IToken::OPERATOR},
        {"'TEST STRING;1+1<>!=@@@###!!'", IToken::STRING_CONST, IToken::SOME_CONST},
        {")", IToken::RPAREN, IToken::OPERATOR},
        {";", IToken::SEMI, IToken::OPERATOR},

        {"end", IToken::END, IToken::KEYWORD},
        {".", IToken::DOT, IToken::OPERATOR},
        {"$", IToken::ENDOFSTREAM, IToken::UNKNOWN},
    };
    checkEqualLex(checkData, lexer, "./build/tests/text3.txt");
}

/// @brief Тест разбора сложной программы на Pascal
TEST(PARSE, PASCAL_HARD){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> checkData{
        {"program", IToken::PROGRAM, IToken::KEYWORD},
        {"test", IToken::ID, IToken::PROGRAM_NAME},
        {";", IToken::SEMI, IToken::OPERATOR},

        {"const", IToken::CONST, IToken::KEYWORD},
        {"x", IToken::ID, IToken::VAR_NAME},
        {"=", IToken::EQ, IToken::OPERATOR},
        {"10", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {";", IToken::SEMI, IToken::OPERATOR},

        {"var", IToken::VAR, IToken::KEYWORD},
        {"a", IToken::ID, IToken::VAR_NAME},
        {",", IToken::COMMA, IToken::OPERATOR},
        {"b", IToken::ID, IToken::VAR_NAME},
        {",", IToken::COMMA, IToken::OPERATOR},
        {"c", IToken::ID, IToken::VAR_NAME},
        {":", IToken::COLON, IToken::OPERATOR},
        {"integer", IToken::INTEGER, IToken::TYPE_SPEC},
        {";", IToken::SEMI, IToken::OPERATOR},

        {"begin", IToken::BEGIN, IToken::KEYWORD},

        {"if", IToken::IF, IToken::KEYWORD},
        {"a", IToken::ID, IToken::VAR_NAME},
        {">", IToken::MORE, IToken::OPERATOR},
        {"b", IToken::ID, IToken::VAR_NAME},
        {":", IToken::COLON, IToken::OPERATOR},
        {"throw", IToken::ID, IToken::FUNCTION_NAME},
        {"'hi'", IToken::STRING_CONST, IToken::SOME_CONST},
        {";", IToken::SEMI, IToken::OPERATOR},

        {"writeln", IToken::ID, IToken::FUNCTION_NAME},
        {"(", IToken::LPAREN, IToken::OPERATOR},
        {"'TEST STRING;1+1<>!=@@@###!!'", IToken::STRING_CONST, IToken::SOME_CONST},
        {")", IToken::RPAREN, IToken::OPERATOR},
        {";", IToken::SEMI, IToken::OPERATOR},

        {"end", IToken::END, IToken::KEYWORD},
        {".", IToken::DOT, IToken::OPERATOR},
        {"$", IToken::ENDOFSTREAM, IToken::UNKNOWN},
    };
    checkEqualLex(checkData, lexer, "./build/tests/text4.txt");
}

/// @brief Тест разбора сложной программы на Pascal
TEST(PARSE, PASCAL_HARD_THROW){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    EXPECT_THROW(lexer.analyzeFile("./build/tests/text5.txt"), LexerException);
}

/// @brief Тест разбора простой программы на Pascal
TEST(PARSE, PASCAL_FOR_TO_LOOP){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> checkData{
        {"program", IToken::PROGRAM, IToken::KEYWORD},
        {"name", IToken::ID, IToken::PROGRAM_NAME},
        {";", IToken::SEMI, IToken::OPERATOR},


        {"begin", IToken::BEGIN, IToken::KEYWORD},

        {"for", IToken::FOR, IToken::KEYWORD},
        {"i", IToken::ID, IToken::VAR_NAME},
        {":=", IToken::ASSIGN, IToken::OPERATOR},
        {"10", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {"to", IToken::TO, IToken::KEYWORD},
        {"20", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {"do", IToken::DO, IToken::KEYWORD},
        {"println", IToken::ID, IToken::FUNCTION_NAME},
        {"(", IToken::LPAREN, IToken::OPERATOR},
        {"'hi'", IToken::STRING_CONST, IToken::SOME_CONST},
        {")", IToken::RPAREN, IToken::OPERATOR},
        {";", IToken::SEMI, IToken::OPERATOR},


        {"end", IToken::END, IToken::KEYWORD},
        {".", IToken::DOT, IToken::OPERATOR},
        {"$", IToken::ENDOFSTREAM, IToken::UNKNOWN},
    };
    checkEqualLex(checkData, lexer, "./build/tests/text6.txt");
}

TEST(PARSE, PASCAL_FOR_DOWN_TO_LOOP){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> checkData{
        {"program", IToken::PROGRAM, IToken::KEYWORD},
        {"name", IToken::ID, IToken::PROGRAM_NAME},
        {";", IToken::SEMI, IToken::OPERATOR},


        {"begin", IToken::BEGIN, IToken::KEYWORD},

        {"for", IToken::FOR, IToken::KEYWORD},
        {"i", IToken::ID, IToken::VAR_NAME},
        {":=", IToken::ASSIGN, IToken::OPERATOR},
        {"10", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {"down to", IToken::DOWNTO, IToken::KEYWORD},
        {"20", IToken::INTEGER_CONST, IToken::SOME_CONST},
        {"do", IToken::DO, IToken::KEYWORD},
        {"println", IToken::ID, IToken::FUNCTION_NAME},
        {"(", IToken::LPAREN, IToken::OPERATOR},
        {"'hi'", IToken::STRING_CONST, IToken::SOME_CONST},
        {")", IToken::RPAREN, IToken::OPERATOR},
        {";", IToken::SEMI, IToken::OPERATOR},


        {"end", IToken::END, IToken::KEYWORD},
        {".", IToken::DOT, IToken::OPERATOR},
        {"$", IToken::ENDOFSTREAM, IToken::UNKNOWN},
    };
    checkEqualLex(checkData, lexer, "./build/tests/text7.txt");
}

int main(void){
    testing::InitGoogleTest();

    return ::RUN_ALL_TESTS();
}