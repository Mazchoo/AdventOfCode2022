
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>

#include "Helpers/FileReading.h"
#include "Helpers/NumpyUtils.h"
#include "Helpers/DataStructures.h"

namespace py = pybind11;

std::list<uint8_t> initLines(std::smatch match) {
    int i = 0;
    std::list<uint8_t> output;

    for (const auto& x : match) {
        if (i == 0) {
            for (auto& ch : (std::string)x)
                output.emplace_back(ch);
        }

        i++;
    }
    return output;
};


struct HillClimbing {
    HillClimbing(std::string& fileName) : mFileName(fileName) {
        std::vector<std::list<uint8_t>> imageLines;
        mFileParsed = parseFileLines<std::list<uint8_t>>(initLines, fileName, mLineRegex, imageLines, 0);
        mFileParsed = mFileParsed && checkAllLinesSameLength(imageLines);
        if (mFileParsed) {
            mHillMap = fillHillMap(imageLines);
            mEndInd = getEndInd();

            // A square is the largest possible similtaneous discovery
            mBuffer1.preallocate(4 * std::min<size_t>(mHeight, mWidth));
            mBuffer2.preallocate(4 * std::min<size_t>(mHeight, mWidth));
            mStorageBuffer = &mBuffer1;
            mGetBuffer = &mBuffer2;

            setUpImage();
        }
    }

    bool checkAllLinesSameLength(std::vector<std::list<uint8_t>> &imageLines) {
        if (imageLines.size() == 0)
            return false;

        size_t prevLength = imageLines[0].size();
        for (size_t i = 1; i < imageLines.size(); ++i) {
            if (imageLines[i].size() != prevLength)
                return false;
        }

        mHeight = imageLines.size();
        mWidth = prevLength;
        return true;
    }

    std::vector<uint8_t> fillHillMap(std::vector<std::list<uint8_t>>& imageLines) {
        std::vector<uint8_t> output;
        output.resize(imageLines.size() * imageLines[0].size());

        size_t i = 0;
        for (auto& line : imageLines) {
            for (auto& ch : line)
                output[i++] = ch;
        }
        return output;
    }

    bool getFileParsed() {
        return mFileParsed;
    };

    py::array showImage() {
        py::array arr = py::array_t<uint8_t>({ mHeight, mWidth });
        auto ptr = getPointerToNumpyData<uint8_t>(arr);
        std::move(mHillMap.begin(), mHillMap.end(), ptr);
        return arr;
    }

    py::array showDists() {
        py::array arr = py::array_t<int32_t>({ mHeight, mWidth });
        auto ptr = getPointerToNumpyData<int32_t>(arr);
        std::move(mDistances.begin(), mDistances.end(), ptr);
        return arr;
    }

    size_t getStartInd() {
        size_t i = 0;
        for (auto& x : mHillMap) {
            if (x == 83) {
                return i;
            }
            i++;
        }
        return 0;
    }

    size_t getEndInd() {
        size_t i = 0;
        for (auto& x : mHillMap) {
            if (x == 69) {
                return i;
            }
            i++;
        }
        return 0;
    }

    bool addPointsToNeighbour(int32_t threshold, int32_t maxDistance, size_t i, size_t imageSize) {
        bool foundAnswer = false;

        int j = i - 1;
        if (i % mWidth != 0 && mDistances[j] < 0 && threshold > mHillMap[j]) {
            mDistances[j] = maxDistance;
            mStorageBuffer->append(j);
            foundAnswer = foundAnswer || (j == mEndInd);
        }
        j = i - mWidth;
        if (j > -1 && mDistances[j] < 0 && threshold > mHillMap[j]) {
            mDistances[j] = maxDistance;
            mStorageBuffer->append(j);
            foundAnswer = foundAnswer || (j == mEndInd);
        }
        j = i + 1;
        if (j % mWidth != 0 && mDistances[j] < 0 && threshold > mHillMap[j]) {
            mDistances[j] = maxDistance;
            mStorageBuffer->append(j);
            foundAnswer = foundAnswer || (j == mEndInd);
        }
        j = i + mWidth;
        if (j < imageSize && mDistances[j] < 0 && threshold > mHillMap[j]) {
            mDistances[j] = maxDistance;
            mStorageBuffer->append(j);
            foundAnswer = foundAnswer || (j == mEndInd);
        }

        return foundAnswer;
    }

    void resetBuffers(size_t ind) {
        mSolved = false;
        mMaxDistance = 0;

        mStorageBuffer->reset();
        mGetBuffer->reset();
        mStorageBuffer->append(ind);

        mDistances = std::vector<int32_t>(mHeight * mWidth, -1);
        mDistances[ind] = 0;
    }

    void setUpImage() {
        size_t startInd = getStartInd();

        mHillMap[startInd] = 97;
        mHillMap[mEndInd] = 122;

        resetBuffers(startInd);
    }

    void swapBuffers() {
        BufferedList<int>* copyBuffer = mStorageBuffer;
        mStorageBuffer = mGetBuffer;
        mGetBuffer = copyBuffer;
    }

    void solveIteration() {
        swapBuffers();
        mStorageBuffer->reset();
        int* ptr = mGetBuffer->getPointer();

        for (int i = 0; i < mGetBuffer->mLength; ++i) {
            int k = ptr[i];
            int32_t threshold = mHillMap[k] + 2;
            mSolved = mSolved || addPointsToNeighbour(threshold, mMaxDistance + 1, k, mHeight * mWidth);
        }
        mMaxDistance++;
    }

    uint32_t solveGrid() {
        size_t imageSize = mHeight * mWidth;
        
        while (!mSolved && mMaxDistance < imageSize) {
            solveIteration();   
        }
        return mMaxDistance;
    }

    uint32_t solveAllStartingPositions() {
        BufferedList<int> startPositionsBuffer;
        startPositionsBuffer.preallocate(mHeight * mWidth);
        int i = 0;
        for (auto& x : mHillMap) {
            if (x == 97)
                startPositionsBuffer.append(i);
            i++;
        }

        uint32_t currentSolution = mWidth * mHeight;

        int* ptr = startPositionsBuffer.getPointer();
        for (i = 0; i < startPositionsBuffer.mLength; ++i) {
            resetBuffers(ptr[i]);
            currentSolution = std::min<uint32_t>(solveGrid(), currentSolution);
        }

        return currentSolution;
    }

    size_t mHeight = 0;
    size_t mWidth = 0;
    int32_t mMaxDistance = 0;
    size_t mEndInd = 0;
    std::string mFileName;
    bool mFileParsed = false;
    bool mSolved = false;
    static const std::regex mLineRegex;

    BufferedList<int> mBuffer1;
    BufferedList<int> mBuffer2;
    BufferedList<int>* mStorageBuffer;
    BufferedList<int>* mGetBuffer;
    std::vector<uint8_t> mHillMap;
    std::vector<int32_t> mDistances;
};
const std::regex HillClimbing::mLineRegex = std::regex("(.*)");


void init_day12(py::module& m) {
    py::class_<HillClimbing>(m, "HillClimbing")
        .def(py::init<std::string&>())
        .def("__bool__", &HillClimbing::getFileParsed)
        .def_property_readonly("image", &HillClimbing::showImage)
        .def_property_readonly("distances", &HillClimbing::showDists)
        .def("path_length", &HillClimbing::solveGrid)
        .def("setup_distances", &HillClimbing::setUpImage)
        .def("do_path_iter", &HillClimbing::solveIteration)
        .def("reset_grid", &HillClimbing::resetBuffers)
        .def("shortest_path_length", &HillClimbing::solveAllStartingPositions)
        ;
}