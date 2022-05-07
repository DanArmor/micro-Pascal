#include <vector>
#include <gtest/gtest.h>

#include "List.hpp"
#include "Lexer.hpp"
#include "PascalRules.hpp"
#include "lexerTestRules.hpp"
#include "SyntExp.hpp"

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
        {"1", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {"+", BaseToken::PLUS, BaseToken::OPERATOR},
        {"1", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {"=", BaseToken::EQ, BaseToken::OPERATOR},
        {"2", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {"3", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {"+", BaseToken::PLUS, BaseToken::OPERATOR},
        {"3", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {"=", BaseToken::EQ, BaseToken::OPERATOR},
        {"6", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {"1984", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {"1", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {"*", BaseToken::MUL, BaseToken::OPERATOR},
        {"9", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {"/", BaseToken::FLOAT_DIV, BaseToken::OPERATOR},
        {"(", BaseToken::LPAREN, BaseToken::OPERATOR},
        {"1", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {"-", BaseToken::MINUS, BaseToken::OPERATOR},
        {"2", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {")", BaseToken::RPAREN, BaseToken::OPERATOR},
        {"$", BaseToken::ENDOFSTREAM, BaseToken::UNKNOWN},
    };
    checkEqualLex(checkData, lexer, "./build/tests/text1.txt");
}

/// @brief Тест разбора простейшей программы на Pascal
TEST(PARSE, PASCAL_SIMPLE){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> checkData{
        {"program", BaseToken::PROGRAM, BaseToken::KEYWORD},
        {"test", BaseToken::ID, BaseToken::PROGRAM_NAME},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},
        {"begin", BaseToken::BEGIN, BaseToken::KEYWORD},
        {"end", BaseToken::END, BaseToken::KEYWORD},
        {".", BaseToken::DOT, BaseToken::OPERATOR},
        {"$", BaseToken::ENDOFSTREAM, BaseToken::UNKNOWN},
    };
    checkEqualLex(checkData, lexer, "./build/tests/text2.txt");
}

/// @brief Тест разбора средней программы на Pascal
TEST(PARSE, PASCAL_MEDIUM){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> checkData{
        {"program", BaseToken::PROGRAM, BaseToken::KEYWORD},
        {"test", BaseToken::ID, BaseToken::PROGRAM_NAME},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},

        {"const", BaseToken::CONST, BaseToken::SOME_CONST},
        {"x", BaseToken::ID, BaseToken::VAR_NAME},
        {"=", BaseToken::EQ, BaseToken::OPERATOR},
        {"10", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},

        {"var", BaseToken::VAR, BaseToken::KEYWORD},
        {"a", BaseToken::ID, BaseToken::VAR_NAME},
        {",", BaseToken::COMMA, BaseToken::OPERATOR},
        {"b", BaseToken::ID, BaseToken::VAR_NAME},
        {",", BaseToken::COMMA, BaseToken::OPERATOR},
        {"c", BaseToken::ID, BaseToken::VAR_NAME},
        {":", BaseToken::COLON, BaseToken::OPERATOR},
        {"integer", BaseToken::INTEGER, BaseToken::TYPE_SPEC},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},

        {"begin", BaseToken::BEGIN, BaseToken::KEYWORD},

        {"a", BaseToken::ID, BaseToken::VAR_NAME},
        {":=", BaseToken::ASSIGN, BaseToken::OPERATOR},
        {"0", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},
        {"b", BaseToken::ID, BaseToken::VAR_NAME},
        {":=", BaseToken::ASSIGN, BaseToken::OPERATOR},
        {"0", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},
        {"c", BaseToken::ID, BaseToken::VAR_NAME},
        {":=", BaseToken::ASSIGN, BaseToken::OPERATOR},
        {"x", BaseToken::ID, BaseToken::VAR_NAME},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},
        {"writeln", BaseToken::ID, BaseToken::FUNCTION_NAME},
        {"(", BaseToken::LPAREN, BaseToken::OPERATOR},
        {"'TEST STRING;1+1<>!=@@@###!!'", BaseToken::STRING_CONST, BaseToken::SOME_CONST},
        {")", BaseToken::RPAREN, BaseToken::OPERATOR},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},

        {"end", BaseToken::END, BaseToken::KEYWORD},
        {".", BaseToken::DOT, BaseToken::OPERATOR},
        {"$", BaseToken::ENDOFSTREAM, BaseToken::UNKNOWN},
    };
    checkEqualLex(checkData, lexer, "./build/tests/text3.txt");
}

/// @brief Тест разбора сложной программы на Pascal
TEST(PARSE, PASCAL_HARD){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> checkData{
        {"program", BaseToken::PROGRAM, BaseToken::KEYWORD},
        {"test", BaseToken::ID, BaseToken::PROGRAM_NAME},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},

        {"const", BaseToken::CONST, BaseToken::KEYWORD},
        {"x", BaseToken::ID, BaseToken::VAR_NAME},
        {"=", BaseToken::EQ, BaseToken::OPERATOR},
        {"10", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},

        {"var", BaseToken::VAR, BaseToken::KEYWORD},
        {"a", BaseToken::ID, BaseToken::VAR_NAME},
        {",", BaseToken::COMMA, BaseToken::OPERATOR},
        {"b", BaseToken::ID, BaseToken::VAR_NAME},
        {",", BaseToken::COMMA, BaseToken::OPERATOR},
        {"c", BaseToken::ID, BaseToken::VAR_NAME},
        {":", BaseToken::COLON, BaseToken::OPERATOR},
        {"integer", BaseToken::INTEGER, BaseToken::TYPE_SPEC},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},

        {"begin", BaseToken::BEGIN, BaseToken::KEYWORD},

        {"if", BaseToken::IF, BaseToken::KEYWORD},
        {"a", BaseToken::ID, BaseToken::VAR_NAME},
        {">", BaseToken::MORE, BaseToken::OPERATOR},
        {"b", BaseToken::ID, BaseToken::VAR_NAME},
        {":", BaseToken::COLON, BaseToken::OPERATOR},
        {"throw", BaseToken::ID, BaseToken::FUNCTION_NAME},
        {"'hi'", BaseToken::STRING_CONST, BaseToken::SOME_CONST},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},

        {"writeln", BaseToken::ID, BaseToken::FUNCTION_NAME},
        {"(", BaseToken::LPAREN, BaseToken::OPERATOR},
        {"'TEST STRING;1+1<>!=@@@###!!'", BaseToken::STRING_CONST, BaseToken::SOME_CONST},
        {")", BaseToken::RPAREN, BaseToken::OPERATOR},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},

        {"end", BaseToken::END, BaseToken::KEYWORD},
        {".", BaseToken::DOT, BaseToken::OPERATOR},
        {"$", BaseToken::ENDOFSTREAM, BaseToken::UNKNOWN},
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
        {"program", BaseToken::PROGRAM, BaseToken::KEYWORD},
        {"name", BaseToken::ID, BaseToken::PROGRAM_NAME},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},


        {"begin", BaseToken::BEGIN, BaseToken::KEYWORD},

        {"for", BaseToken::FOR, BaseToken::KEYWORD},
        {"i", BaseToken::ID, BaseToken::VAR_NAME},
        {":=", BaseToken::ASSIGN, BaseToken::OPERATOR},
        {"10", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {"to", BaseToken::TO, BaseToken::KEYWORD},
        {"20", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {"do", BaseToken::DO, BaseToken::KEYWORD},
        {"println", BaseToken::ID, BaseToken::FUNCTION_NAME},
        {"(", BaseToken::LPAREN, BaseToken::OPERATOR},
        {"'hi'", BaseToken::STRING_CONST, BaseToken::SOME_CONST},
        {")", BaseToken::RPAREN, BaseToken::OPERATOR},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},


        {"end", BaseToken::END, BaseToken::KEYWORD},
        {".", BaseToken::DOT, BaseToken::OPERATOR},
        {"$", BaseToken::ENDOFSTREAM, BaseToken::UNKNOWN},
    };
    checkEqualLex(checkData, lexer, "./build/tests/text6.txt");
}

TEST(PARSE, PASCAL_FOR_DOWN_TO_LOOP){
    Lexer lexer;
    lexer.setTemplates(PascalRules::getPascalTemplates());
    List<Token> checkData{
        {"program", BaseToken::PROGRAM, BaseToken::KEYWORD},
        {"name", BaseToken::ID, BaseToken::PROGRAM_NAME},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},


        {"begin", BaseToken::BEGIN, BaseToken::KEYWORD},

        {"for", BaseToken::FOR, BaseToken::KEYWORD},
        {"i", BaseToken::ID, BaseToken::VAR_NAME},
        {":=", BaseToken::ASSIGN, BaseToken::OPERATOR},
        {"10", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {"down to", BaseToken::DOWNTO, BaseToken::KEYWORD},
        {"20", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST},
        {"do", BaseToken::DO, BaseToken::KEYWORD},
        {"println", BaseToken::ID, BaseToken::FUNCTION_NAME},
        {"(", BaseToken::LPAREN, BaseToken::OPERATOR},
        {"'hi'", BaseToken::STRING_CONST, BaseToken::SOME_CONST},
        {")", BaseToken::RPAREN, BaseToken::OPERATOR},
        {";", BaseToken::SEMI, BaseToken::OPERATOR},


        {"end", BaseToken::END, BaseToken::KEYWORD},
        {".", BaseToken::DOT, BaseToken::OPERATOR},
        {"$", BaseToken::ENDOFSTREAM, BaseToken::UNKNOWN},
    };
    checkEqualLex(checkData, lexer, "./build/tests/text7.txt");
}

int main(void){
    testing::InitGoogleTest();

    return ::RUN_ALL_TESTS();
}