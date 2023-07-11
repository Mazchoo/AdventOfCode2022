
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

#include "Helpers/FileReading.h"
#include "Helpers/DataStructures.h"

namespace py = pybind11;

typedef std::pair<int, int> ledgePair;
typedef std::vector<ledgePair> ledgeVector;


ledgePair parseLedge(std::string& listString) {
    std::list<std::string> splitPair = splitByWord(listString, ",");

    if (splitPair.size() != 2)
        return ledgePair(500, 0);

    return ledgePair(std::stoi(splitPair.front()), std::stoi(splitPair.back()));
}


ledgeVector initLedgeVector(std::smatch match) {
    if (match.empty())
        return {};

    std::string line = match[0];

    std::list<std::string> allLedgeStrings = splitByWord(line, " -> ");
    ledgeVector output;
    output.resize(allLedgeStrings.size());

    int i = 0;
    for (auto& strPair : allLedgeStrings)
        output[i++] = parseLedge(strPair);

    return output;
};


struct FallingSandGame {
    FallingSandGame(std::string& fileName) : mFileName(fileName) {
        mLedges.clear();
        mFileParsed = parseFileLines<ledgeVector>(initLedgeVector, fileName, mLineRegex, mLedges, 0);
    }

    bool getFileParsed() {
        return mFileParsed;
    };

    std::string mFileName;
    bool mFileParsed = false;
    static const std::regex mLineRegex;

    std::vector<ledgeVector> mLedges;
};
const std::regex FallingSandGame::mLineRegex = std::regex(".*");


void init_day14(py::module& m) {
    py::class_<FallingSandGame>(m, "FallingSandGame")
        .def(py::init<std::string&>())
        .def("__bool__", &FallingSandGame::getFileParsed)
        ;
}