
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

static const int MAX_ITERATIONS = 1000000;


ledgePair parseLedge(std::string& listString) {
    std::list<std::string> splitPair = splitByWord(listString, ",");

    if (splitPair.size() != 2)
        return ledgePair(0, 0);

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
    FallingSandGame(std::string& fileName, size_t startX, size_t startY) :
        mFileName(fileName), mStartX(startX), mStartY(startY) {
        mLedges.clear();
        mFileParsed = parseFileLines<ledgeVector>(initLedgeVector, fileName, mLineRegex, mLedges, 0);

        constructLedgeImage();
    }

    bool getFileParsed() const {
        return mFileParsed;
    };

    bool canMove() const {
        return mCanMove;
    };

    int nrSand() const {
        return mNrSandAtRest;
    };

    void findImageDimensions() {
        size_t maxX = mStartX;
        size_t minX = mStartX;
        size_t maxY = mStartY;
        size_t minY = mStartY;

        for (auto& ledgeVector : mLedges) {
            for (auto& coord : ledgeVector) {
                minX = std::min<int>(minX, coord.second);
                maxX = std::max<int>(maxX, coord.second);
                minY = std::min<int>(minY, coord.first);
                maxY = std::max<int>(maxY, coord.first);
            }
        }

        mWidth = maxX - minX + 1;
        mHeight = maxY - minY + 1;
        mMinX = minX;
        mMinY = minY;
    }

    int getImageCoord(ledgePair& coord) {
        return (coord.first - mMinY) * mWidth + coord.second - mMinX;
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

    bool update() {
        if (!mCanMove)
            return false;

        if (mGameAtRest) {
            auto startCoord = getImageCoord(ledgePair(mStartY, mStartX));
            mCurrentX = mStartX;
            mCurrentY = mStartY;

            if (mLedgeGrid[startCoord] == 0) {
                mLedgeGrid[startCoord] = 2;
                mGameAtRest = false;
                return true;
            } else {
                mLedgeGrid[startCoord] = 3;
                mCanMove = false;
                return false;
            }
        }

        int currentCordinate = getImageCoord(ledgePair(mCurrentY, mCurrentX));
        int lowerCoordinate = currentCordinate + mWidth;
        // Sand cannot move down without falling into abyss
        if (lowerCoordinate >= mHeight * mWidth) {
            mCanMove = false;
            return false;
        }
        // Sand can move down one square
        if (mLedgeGrid[lowerCoordinate] == 0) {
            mLedgeGrid[currentCordinate] = 0;
            mLedgeGrid[lowerCoordinate] = 2;
            mCurrentY++;
            return true;
        }

        int lowerLeftCoorindate = lowerCoordinate - 1;
        // Sand moved outside left most coordinate
        if (lowerLeftCoorindate % mWidth == mWidth - 1) {
            mCanMove = false;
            mLedgeGrid[currentCordinate] = 3;
            return false;
        }
        // Sand can move left down
        if (mLedgeGrid[lowerLeftCoorindate] == 0) {
            mLedgeGrid[currentCordinate] = 0;
            mLedgeGrid[lowerLeftCoorindate] = 2;
            mCurrentY++;
            mCurrentX--;
            return true;
        }

        int lowerRightCoorindate = lowerCoordinate + 1;
        // Sand moved outside left most coordinate
        if (lowerRightCoorindate % mWidth == 0) {
            mCanMove = false;
            mLedgeGrid[currentCordinate] = 3;
            return false;
        }
        // Sand can move left right
        if (mLedgeGrid[lowerRightCoorindate] == 0) {
            mLedgeGrid[currentCordinate] = 3;
            mLedgeGrid[currentCordinate] = 0;
            mLedgeGrid[lowerRightCoorindate] = 2;
            mCurrentY++;
            mCurrentX++;
            return true;
        }

        mNrSandAtRest++;
        mGameAtRest = true;
        return true;
    }

    bool updateUntilAtRest() {
        if (mGameAtRest)
            update();

        if (!mCanMove)
            return false;

        int iteration = 0;
        while (iteration++ < MAX_ITERATIONS && !mGameAtRest)
            update();

        return true;
    }

    std::string mFileName;
    bool mFileParsed = false;
    static const std::regex mLineRegex;

    std::vector<ledgeVector> mLedges;
    std::vector<uint8_t> mLedgeGrid;

    size_t mStartX = 0;
    size_t mStartY = 0;
    size_t mCurrentX = 0;
    size_t mCurrentY = 0;

    size_t mWidth = 0;
    size_t mHeight = 0;
    size_t mMinX = 0;
    size_t mMinY = 0;

    int mNrSandAtRest = 0;
    bool mGameAtRest = true;
    bool mCanMove = true;
};
const std::regex FallingSandGame::mLineRegex = std::regex(".*");


void init_day14(py::module& m) {
    py::class_<FallingSandGame>(m, "FallingSandGame")
        .def(py::init<std::string&, int, int>())
        .def("__bool__", &FallingSandGame::getFileParsed)
        .def_property_readonly("image", &FallingSandGame::showImage)
        .def_property_readonly("canMove", &FallingSandGame::canMove)
        .def_property_readonly("nrSand", &FallingSandGame::nrSand)
        .def("update", &FallingSandGame::update)
        .def("updateUntilAtRest", &FallingSandGame::updateUntilAtRest)
        ;
}