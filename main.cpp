
#include "Year2022Day01.hpp"
#include "Year2022Day02.hpp"
#include "Year2022Day03.hpp"
#include "Year2022Day04.hpp"
#include "Year2022Day05.hpp"
#include "Helpers/FileReading.h"
#include <pybind11/pybind11.h>
#include <iostream>
#include <fstream>
#include <string>

namespace py = pybind11;

PYBIND11_MODULE(AdventModule, m) {
     m.doc() = "This module has a doc.";
     init_day01(m);
     init_day02(m);
     init_day03(m); 
     init_day04(m);
     init_day05(m);
}

int main() {
     std::cout << "Starting Program" << std::endl;
     std::cout << RootFolder() << std::endl;
     std::string inputFile = RootFolder() + "data/2022Day05/small_input.txt";

     auto movingStacks = MovingStacks(inputFile);
     std::list<std::list<char>> initStack = { {'N', 'Z'}, {'D', 'C', 'M'}, {'P'} };
     movingStacks.setStack(initStack);
     movingStacks.doAllMovesKeepOrder();

     std::cin.get();
     return 0;
}