
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <vector>

typedef std::pair<std::string, std::string> rucksack;

namespace py = pybind11;

class RucksackContents {
     public:
        RucksackContents(std::string fileName): mFileName(fileName) {
            std::fstream file;
            file.open(mFileName, std::ios::in);
            if (file.is_open()){
                std::string line;
                std::list<rucksack> rucksackList;

                while(getline(file, line)) {
                    size_t lineLength = line.length();
                    std::string firstPart = line.substr(0, lineLength / 2);
                    std::string secondPart = line.substr(lineLength / 2, lineLength);
                    rucksackList.emplace_back(std::make_pair(firstPart, secondPart));
                }             

                file.close();
                mFileParsed = true;
                mRucksacks = std::vector<rucksack>(rucksackList.begin(), rucksackList.end());
            }
        }

        std::list<py::tuple> getRuckSacks() {
            std::list<py::tuple> outputList;
            for (auto& sack : mRucksacks)
                outputList.emplace_back(py::make_tuple(sack.first, sack.second));
            return outputList;
        }

        std::vector<char> getSharedCharacters() {
            std::vector<char> outputVec(mRucksacks.size());
            int i = 0;
            for (auto& sack : mRucksacks)
                outputVec[i++] = getCommonChar(sack);
            return outputVec;
        }

        std::vector<char> getSharedGroupCharacters() {
            std::vector<char> outputVec(mRucksacks.size() / 3);
            for (size_t i = 0; i < mRucksacks.size() / 3; ++i) {
                rucksack sack1 = mRucksacks[3 * i];
                rucksack sack2 = mRucksacks[3 * i + 1];
                rucksack sack3 = mRucksacks[3 * i + 2];

                outputVec[i] = getCommonFromThreeSacks(sack1, sack2, sack3);
            }
                
            return outputVec;
        }

        int getTotalError() {
            std::vector<char> sharedChars = getSharedCharacters();
            int totalError = 0;
            for (auto& character : sharedChars)
                totalError += getCharValue(character);
            return totalError;
        }

        int getSharedGroupError() {
            std::vector<char> sharedChars = getSharedGroupCharacters();
            int totalError = 0;
            for (auto& character : sharedChars)
                totalError += getCharValue(character);
            return totalError;
        }

        bool getFileParsed() {
            return mFileParsed;
        }

     private:
          std::string mFileName;
          std::vector<rucksack> mRucksacks;
          bool mFileParsed = false;
            
          char getCommonChar(rucksack sack) {
              for (char character : sack.first) {
                  size_t found = sack.second.find(character);
                  if (found != std::string::npos)
                      return character;
              }
              std::cout << "No common char " << sack.first << ", " << sack.second << std::endl;
              return 0;
          }

          char getCommonFromThreeSacks(rucksack sack1, rucksack sack2, rucksack sack3) {
              std::string sack1String = sack1.first + sack1.second;
              std::string sack2String = sack2.first + sack2.second;
              std::string sack3String = sack3.first + sack3.second;

              for (char character : sack1String) {
                  size_t found1 = sack2String.find(character);
                  size_t found2 = sack3String.find(character);
                  if (found1 != std::string::npos && found2 != std::string::npos)
                      return character;
              }

              std::cout << "No common char " << sack1String << ", " << sack2String << ", " << sack3String << std::endl;
              return 0;
          }

          int getCharValue(char character) {
              if (character > 96) {
                  return static_cast<int>(character - 96);
              }
              else {
                  return static_cast<int>(character - 64 + 26);
              }
          }
};

void init_day03(py::module &m) {
     py::class_<RucksackContents>(m, "RucksackContents")
          .def(py::init<std::string&>())
          .def("__bool__", &RucksackContents::getFileParsed)
          .def_property_readonly("get_rucksacks", &RucksackContents::getRuckSacks)
          .def("find_shared_characters", &RucksackContents::getSharedCharacters)
          .def("calculate_total_error", &RucksackContents::getTotalError)
          .def("find_shared_shared_group_chars", &RucksackContents::getSharedGroupCharacters)
          .def("calculate_total_group_error", &RucksackContents::getSharedGroupError)
     ;
}