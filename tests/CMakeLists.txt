project(tests)

set(CMAKE_BUILD_TYPE Debug)

list(APPEND LIBS "-lgtest" "-lpthread")
# Тесты списка
add_executable(testList structures/testList.cpp)
target_link_libraries(testList ${LIBS})

# Тесты лексера
add_executable(testLexer_1 lexer/testLexer_1.cpp lexer/lexerTestRules.hpp)
target_link_libraries(testLexer_1 ${LIBS} Lexer fmt SyntExp Visitor)

# Тесты синтаксического анализатора
add_executable(testSyntax syntax/testSyntax.cpp)
target_link_libraries(testSyntax ${LIBS} SyntExp Lexer fmt PascalSyntax AST Visitor)

# Тесты синтаксического анализатора и сематники
add_executable(testSemantic semantic/testSemantic.cpp)
target_link_libraries(testSemantic ${LIBS} SyntExp Lexer fmt PascalSyntax AST Visitor)