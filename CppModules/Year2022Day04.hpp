
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <regex>

#include "Helpers/FileReading.h"

typedef std::array<int, 2> range;
typedef std::pair<range, range> rota;

namespace py = pybind11;

rota rotaConstruct(std::smatch match) {
    int i = -1;
    range range1;
    range range2;

    for (const auto& x : match) {
        if (i != -1) {
            if (i < 2) {
                range1[i] = std::stoi(x);
            }
            else {
                range2[i % 2] = std::stoi(x);
            }
        } 
        i++;
    }
    return rota{ range1, range2 };
};


struct CleaningRotas {
    CleaningRotas(std::string fileName) : mFileName(fileName) {
        mFileParsed = parseFileLines<rota>(rotaConstruct, fileName, mRegexExp, mRotas, 0);
    }

    bool getFileParsed() {
        return mFileParsed;
    }

    int getNumberOfContains() {
        int totalContains = 0;
        for (auto& r : mRotas)
            totalContains += rotaFullContains(r) ? 1 : 0;

        return totalContains;
    }

    int getNumberOfOverlaps() {
        int totalOverlaps = 0;
        for (auto& r : mRotas)
            totalOverlaps += rotaOverlaps(r) ? 1 : 0;

        return totalOverlaps;
    }

    bool rotaFullContains(rota r) {
        return (r.first[0] >= r.second[0] && r.first[1] <= r.second[1]) || (r.second[0] >= r.first[0] && r.second[1] <= r.first[1]);
    }

    bool rotaOverlaps(rota r) {
        return !(r.first[1] < r.second[0] || r.second[1] < r.first[0]);
    }

    std::vector<rota> mRotas;
    std::string mFileName;
    bool mFileParsed = false;
    static const std::regex mRegexExp;
};
const std::regex CleaningRotas::mRegexExp = std::regex("([0-9]+)-([0-9]+),([0-9]+)-([0-9]+)");


void init_day04(py::module& m) {
    py::class_<CleaningRotas>(m, "CleaningRotas")
        .def(py::init<std::string>())
        .def("__bool__", &CleaningRotas::getFileParsed, py::return_value_policy::copy)
        .def("__iter__", [](CleaningRotas &self) {
            return py::make_iterator(self.mRotas.begin(), self.mRotas.end());
        })
        .def("get_number_fully_cotained", &CleaningRotas::getNumberOfContains)
        .def("get_number_overlaps", &CleaningRotas::getNumberOfOverlaps)
        ;
}