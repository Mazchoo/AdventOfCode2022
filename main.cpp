
#include "CppModules/Year2022Day01.hpp"
#include "CppModules/Year2022Day02.hpp"
#include "CppModules/Year2022Day03.hpp"
#include "CppModules/Helpers/FileReading.h"
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
}

int main() {
     std::cout << "Starting Program" << std::endl;
     std::cout << RootFolder() << std::endl;
     std::string inputFile = RootFolder() + "data/2022Day03/small_input.txt";
     auto rucksacks = RucksackContents(inputFile);
     rucksacks.getSharedGroupCharacters();
     std::cin.get();
     return 0;
}