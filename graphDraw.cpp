#include <iostream>
#include "Analyze.hpp"

int main(int argc, char** argv){

    if(argc < 2 || argc > 3){
        std::cout << "Пожалуйста, введите только\033[31m название исходного файла\33[0m для разбора после названия программы\n. Опциально можно указать имя выходного файла\n";
        exit(1);
    }
    if(argc == 2)
        Analyze::analyzeFile(argv[1]);
    else
        Analyze::analyzeFile(argv[1], argv[2]);

    return 0;
}
