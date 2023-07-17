
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

static const size_t SOURCE_X = 500;
static const size_t SOURCE_Y = 0;


ledgePair parseLedge(std::string& listString) {
    std::list<std::string> splitPair = splitByWord(listString, ",");

    if (splitPair.size() != 2)
        return ledgePair(SOURCE_Y, SOURCE_X);

    return ledgePair(std::stoi(splitPair.back()), std::stoi(splitPair.front()));
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

        constructLedgeImage();
    }

    bool getFileParsed() {
        return mFileParsed;
    };

    void findImageDimensions() {
        size_t maxX = SOURCE_X;
        mHeight = SOURCE_Y;

        for (auto& ledgeVector : mLedges) {
            for (auto& coord : ledgeVector) {
                mXOffset = std::min<int>(mXOffset, coord.second);
                maxX = std::max<int>(maxX, coord.second);
                mHeight = std::max<int>(mHeight, coord.first);
            }
        }

        mWidth = maxX - mXOffset + 1;
        mHeight += 1;
    }

    int getImageCoord(ledgePair& coord) {
        return coord.first * mWidth + coord.second - mXOffset;
    }

    void drawPointPair(uint8_t* imagePtr, ledgePair& prevCoord, ledgePair& coord) {
        if (prevCoord.first != coord.first) {
            int minY = std::min<int>(prevCoord.first, coord.first);
            int maxY = std::max<int>(prevCoord.first, coord.first);

            for (int y = minY; y <= maxY; ++y) {
                ledgePair drawPoint = { y, coord.second };
                imagePtr[getImageCoord(drawPoint)] = 1;
            }
                
        }
        else if (prevCoord.second != coord.second) {
            int minX = std::min<int>(prevCoord.second, coord.second);
            int maxX = std::max<int>(prevCoord.second, coord.second);

            for (int x = minX; x <= maxX; ++x) {
                ledgePair drawPoint = { coord.first, x };
                imagePtr[getImageCoord(drawPoint)] = 1;
            }
        }   
    }

    void constructLedgeImage() {
        findImageDimensions();
        mLedgeGrid = std::vector<uint8_t>(mHeight * mWidth, 0);
        uint8_t* ptr = mLedgeGrid.data();

        for (auto& ledgeVector : mLedges) {
            if (ledgeVector.size() > 0) {
                ledgePair& prevCoord = ledgeVector[0];
                bool previousPairExists = false;

                for (auto& coord : ledgeVector) {
                    if (previousPairExists)
                        drawPointPair(ptr, prevCoord, coord);

                    prevCoord = coord;
                    previousPairExists = true;
                }
            }
        }
    }

    py::array showImage() {
        return constructNumpyArray<uint8_t>(mLedgeGrid, mHeight, mWidth);
    }

    std::string mFileName;
    bool mFileParsed = false;
    static const std::regex mLineRegex;

    std::vector<ledgeVector> mLedges;
    std::vector<uint8_t> mLedgeGrid;

    size_t mXOffset = SOURCE_X;
    size_t mWidth = 0;
    size_t mHeight = 0;
};
const std::regex FallingSandGame::mLineRegex = std::regex(".*");


void init_day14(py::module& m) {
    py::class_<FallingSandGame>(m, "FallingSandGame")
        .def(py::init<std::string&>())
        .def("__bool__", &FallingSandGame::getFileParsed)
        .def_property_readonly("image", &FallingSandGame::showImage)
        ;
}