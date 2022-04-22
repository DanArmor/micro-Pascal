cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)

project(lab)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)
include_directories(side_include)

# Список
add_library(List include/List.cpp)

# Очередь
add_library(Queue include/Queue.cpp)

# Стек
add_library(Stack include/Stack.cpp)


add_library(Visitor src/Visitor.cpp)
add_library(Token src/Token.cpp)
add_library(AST src/AST.cpp)
add_library(Syntax src/Syntax.cpp)
add_library(Lexer src/Lexer.cpp)
add_library(ASTFactory src/ASTFactory.cpp)
add_library(SyntExp src/SyntExp.cpp)

target_link_libraries(Token fmt)
target_link_libraries(Visitor fmt AST Token)
target_link_libraries(AST Visitor Token)
target_link_libraries(Lexer Token)
target_link_libraries(Syntax AST Token ASTFactory fmt SyntExp)
target_link_libraries(ASTFactory AST Token fmt)
target_link_libraries(SyntExp Token fmt)

# Программа
add_executable(Main main.cpp)
target_link_libraries(Main Visitor Token AST Syntax Lexer SyntExp)

add_executable(graphDraw graphDraw.cpp)
target_link_libraries(graphDraw Visitor Token AST Syntax Lexer SyntExp)

add_subdirectory(tests)