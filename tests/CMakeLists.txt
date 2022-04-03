project(tests)

set(CMAKE_BUILD_TYPE Debug)
# Тесты списка
add_executable(testList structures/testList.cpp)
target_link_libraries(testList -lgtest)

# Тесты очереди
add_executable(testQueue structures/testQueue.cpp)
target_link_libraries(testQueue -lgtest)

# Тесты Стека
add_executable(testStack structures/testStack.cpp)
target_link_libraries(testStack -lgtest)

# Тесты лексера
add_executable(testLexer_1 lexer/testLexer_1.cpp lexer/lexerTestRules.hpp)
target_link_libraries(testLexer_1 -lgtest Lexer fmt)

# Тесты синтаксического анализатора
add_executable(testSyntax syntax/testSyntax.cpp)
target_link_libraries(testSyntax -lgtest Lexer fmt Syntax AST)