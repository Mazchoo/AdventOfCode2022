
#include "CppModules/Year2022Day01.hpp"
#include "CppModules/Year2022Day02.hpp"
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
}

int main() {
     std::cout << "Starting Program" << std::endl;
     std::cout << RootFolder() << std::endl;
     std::string inputFile = RootFolder() + "/data/2022Day02/small_example.txt";
     auto games = RockPaperScissorGames(inputFile);
     std::cin.get();
     return 0;
}