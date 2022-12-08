
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <fstream>
#include <string>
#include <list>
#include <vector>

#include "Helpers/NumpyUtils.h"

namespace py = pybind11;

class CalorieCollection {
     public:
          CalorieCollection(std::string fileName): mFileName(fileName) {
               std::fstream file;
               file.open(mFileName, std::ios::in);
               if (file.is_open()){
                    std::string line;
                    std::list<int> currentList;
                    std::vector<int> currentVector;
                    while(getline(file, line))
                    {
                         if (line.length() > 0) {
                              currentList.emplace_back(std::stoi(line));
                         } else {
                              currentVector = std::vector<int>(currentList.begin(), currentList.end());
                              mCalories.emplace_back(currentVector);
                              currentList = {};
                         }
                    }
                    currentVector = std::vector<int>(currentList.begin(), currentList.end());
                    mCalories.emplace_back(currentVector);
                    file.close();
               }
          }

          py::array totals() {
               py::array arr = py::array_t<int>(mCalories.size());
               auto ptr = getPointerToNumpyData<int>(arr);

               int i = 0;
               for (auto& vec : mCalories) {
                    int total = 0;
                    for (auto& x : vec)
                         total += x;
                    ptr[i++] = total;
               }

               std::sort(ptr, ptr + i);

               return arr;
          }
     private:
          std::string mFileName;
          std::list<std::vector<int>> mCalories;
};

void init_day01(py::module &m) {
     py::class_<CalorieCollection>(m, "CalorieCollection")
          .def(py::init<std::string&>())
          .def_property_readonly("totals", [](CalorieCollection &self) {
               return self.totals();
          })
     ;
}
