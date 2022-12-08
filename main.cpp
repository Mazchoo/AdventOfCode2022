
#include "Year2022Day01.hpp"
#include "Year2022Day02.hpp"
#include "Year2022Day03.hpp"
#include "Year2022Day04.hpp"
#include "Year2022Day05.hpp"
#include "Year2022Day06.hpp"
#include "Year2022Day07.hpp"
#include "Year2022Day08.hpp"
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
     init_day06(m);
     init_day07(m);
     init_day08(m);
}

int main() {
     std::cout << "Starting Program" << std::endl;
     std::cout << RootFolder() << std::endl;
     std::string inputFile = RootFolder() + "data/2022Day08/small_input.txt";

     [[maybe_unused]] auto treeMap = TreeMap(inputFile);
     [[maybe_unused]] auto visibility = treeMap.getScenicScoreImage();

     std::cin.get();
     return 0;
}