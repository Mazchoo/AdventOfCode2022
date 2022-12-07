
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>

namespace py = pybind11;

std::map<std::string, int32_t> GAME_OUTCOMES = {
    {"A X", 4}, // Draw plus 1
    {"A Y", 8}, // Win plus 2
    {"A Z", 3}, // Lose plus 3
    {"B X", 1}, // Lose plus 1
    {"B Y", 5}, // Draw plus 2
    {"B Z", 9}, // Win plus 3
    {"C X", 7}, // Win plus 1
    {"C Y", 2}, // Lose plus 2
    {"C Z", 6}, // Draw plus 3
};

std::array<int32_t, 9> SCORES = {1, 6, 8, 3, 5, 7, 2, 4, 9};

class RockPaperScissorGames {
     public:
        RockPaperScissorGames(std::string fileName): mFileName(fileName) {
            std::fstream file;
            file.open(mFileName, std::ios::in);
            if (file.is_open()){
                std::string line;
                std::list<int32_t> outComesList;

                while(getline(file, line)) {
                    outComesList.emplace_back(GAME_OUTCOMES[line]);
                }             

                file.close();
                mFileParsed = true;
                mOutcomes = std::vector<int32_t>(outComesList.begin(), outComesList.end());
            }
        }

        int totalScore() {
            int32_t totalScore = 0;
            for (auto& outcome : mOutcomes)
                totalScore += outcome;
            return totalScore;
        }

        int totalRealScore() {
            int32_t totalScore = 0;
            for (auto& outcome : mOutcomes)
                totalScore += SCORES[(size_t)outcome - 1];
            return totalScore;
        }

        bool getFileParsed() {
            return mFileParsed;
        }

     private:
          std::string mFileName;
          std::vector<int32_t> mOutcomes;
          bool mFileParsed = false;
};

// C++ manages the life time of GAME_OUTCOMES and SCORES
void init_day02(py::module &m) {
     py::class_<RockPaperScissorGames>(m, "RockPaperScissorGames")
          .def(py::init<std::string>())
          .def("total_score", &RockPaperScissorGames::totalScore)
          .def("total_real_score", &RockPaperScissorGames::totalRealScore)
          .def("get_map", []() {return GAME_OUTCOMES;}, py::return_value_policy::reference)
          .def("get_score_map", []() {return SCORES;}, py::return_value_policy::reference)
          .def_property_readonly("get_parsed", [](RockPaperScissorGames &self) {
               return self.getFileParsed();
          })
     ;
}