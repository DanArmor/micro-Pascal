cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)

project(lab)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)
include_directories(side_include)

add_library(Visitor src/Visitor.cpp)
add_library(Token src/Token.cpp)
add_library(AST src/AST.cpp)
add_library(Lexer src/Lexer.cpp)
add_library(ASTFactory src/ASTFactory.cpp)
add_library(SyntExp src/SyntExp.cpp)
add_library(Analyzer src/Analyzer.cpp)
add_library(PascalSyntax src/PascalSyntax.cpp)

target_link_libraries(Token fmt)
target_link_libraries(Visitor fmt AST Token)
target_link_libraries(AST Visitor Token)
target_link_libraries(Lexer Token)
target_link_libraries(PascalSyntax AST Token ASTFactory fmt SyntExp)
target_link_libraries(ASTFactory AST Token fmt)
target_link_libraries(SyntExp Token fmt)
target_link_libraries(Analyzer PascalSyntax Token AST SyntExp fmt Lexer)

# Программа
add_executable(Main main.cpp)
target_link_libraries(Main Analyzer)

add_executable(graphDraw graphDraw.cpp)
target_link_libraries(graphDraw Analyzer)

add_subdirectory(tests)