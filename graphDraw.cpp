#include <iostream>
#include "Analyze.hpp"

int main(int argc, char** argv){

    if(argc != 2){
        std::cout << "Пожалуйста, введите только\033[31m название исходного файла\33[0m для разбора после названия программы\n";
        exit(1);
    }

    Analyze::analyzeFile(argv[1]);

    return 0;
}
