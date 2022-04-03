// Пример работы посетителя на АДС выражения, разобранного классом SyntaxAnalyzer
SyntaxAnalyzer syntax(tokens);
AST *root = syntax.parseTokens(); // Получаем корень дерева АДС

GraphvizVisitor graph("out.dot"); //  Создаем посетителя

root->accept(graph); // Просим корень принять посетителя
graph.write(); // Посетитель закончил работу