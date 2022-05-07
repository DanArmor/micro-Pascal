#include <iostream>
#include <map>
#include <string>

#include "Analyzer.hpp"
#include "PascalRules.hpp"
#include "PascalSyntax.hpp"
#include "Visitor.hpp"

void analyzePascalProgram(std::string fileName) {
    auto semantic = std::make_unique<SemanticVisitor>();
    semantic->prebuildFunctions(PascalRules::getStandartFuncs());
    auto graph = std::make_unique<GraphvizVisitor>("out.dot");
    Analyzer analyzer(std::make_unique<PascalSyntaxAnalyzer>(),
                      PascalRules::getPascalTemplates());
    analyzer.addVisitor(std::move(semantic));
    analyzer.addVisitor(std::move(graph));
    analyzer.analyzeFile(fileName);
    analyzer.highlightOutput();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Пожалуйсrа, введите только\033[31m название исходного "
                     "файла\33[0m для разбора после названия программы\n.";
        exit(1);
    }
    analyzePascalProgram(argv[1]);

    return 0;
}
