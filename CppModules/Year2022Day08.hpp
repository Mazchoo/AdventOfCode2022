
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

#include "Helpers/FileReading.h"
#include "Helpers/NumpyUtils.h"

namespace py = pybind11;

std::vector<uint8_t> initTreeLine(std::smatch match) {
    int i = 0;
    std::vector<uint8_t> output;
    output.resize(match.size() - 1);

    for (const auto& x : match) {
        if (i > 0)
            output[i - 1] = std::stoi(x);
        i++;
    }
    return output;
};


struct TreeMap {
    TreeMap(std::string fileName) : mFileName(fileName) {
        if (!getPitch(fileName))
            return;

        std::string charRegex = "";
        for (int i = 0; i < mPitch; ++i)
            charRegex += "([0-9])";

        mFileParsed = parseFileLines<std::vector<uint8_t>>(initTreeLine, fileName, std::regex(charRegex), mTreeImage, 0);
        if (mFileParsed) {
            mMaxTop   = getMaxTop();
            mMaxRight = getMaxRight();
            mMaxLeft  = getMaxLeft();
            mMaxDown  = getMaxDown();
        }
        else {
            mPitch = 0;
        }
    }

    bool getFileParsed() {
        return mFileParsed;
    };

    bool getPitch(std::string& fileName) {
        std::fstream file;
        file.open(fileName, std::ios::in);
        if (!file.is_open())
            return false;

        bool lineExists = false;
        std::string line;
        if (getline(file, line)) {
            mPitch = line.length();
            lineExists = true;
        }

        file.close();
        return lineExists;
    }

    py::array showImage() {
        py::array arr = py::array_t<uint8_t>({ mTreeImage.size(), mPitch });
        auto ptr = getPointerToNumpyData<uint8_t>(arr);
        size_t i = 0;
        for (auto& line : mTreeImage) {
            for (auto& x : line)
                ptr[i++] = x;
        }
        return arr;
    }

    py::array moveArrayToNumpy(std::vector<uint8_t> sourceVector) {
        py::array arr = py::array_t<uint8_t>({ mTreeImage.size(), mPitch });
        auto ptr = getPointerToNumpyData<uint8_t>(arr);
        std::move(sourceVector.begin(), sourceVector.end(), ptr);
        return arr;
    }

    py::array showUpMax() {
        return moveArrayToNumpy(mMaxTop);
    }

    py::array showDownMax() {
        return moveArrayToNumpy(mMaxDown);
    }

    py::array showRightMax() {
        return moveArrayToNumpy(mMaxRight);
    }

    py::array showLeftMax() {
        return moveArrayToNumpy(mMaxLeft);
    }

    std::vector<uint8_t> getMaxTop() {
        size_t treeMapSize = mTreeImage.size() * mPitch;
        std::vector<uint8_t> maxTop(treeMapSize, 0);
        for (size_t i = 1; i < mPitch - 1; ++i)
        {
            size_t ind = treeMapSize - 2 * mPitch + i;
            maxTop[ind] = mTreeImage[mTreeImage.size() - 1][i];
        }
        for (size_t i = 2; i < mTreeImage.size() - 1; ++i)
        {
            for (int j = 1; j < mPitch - 1; ++j) {
                size_t ind = treeMapSize - (i + 1) * mPitch + j;
                maxTop[ind] = std::max<uint8_t>(maxTop[ind + mPitch], mTreeImage[mTreeImage.size() - i][j]);
            }
        }
        return maxTop;
    }

    std::vector<uint8_t> getMaxRight() {
        size_t treeMapSize = mTreeImage.size() * mPitch;
        std::vector<uint8_t> maxRight(treeMapSize, 0);
        for (size_t i = 1; i < mPitch - 1; ++i)
        {
            maxRight[mPitch - 2 + mPitch * i] = mTreeImage[i][mPitch - 1];
        }
        for (size_t i = 1; i < mTreeImage.size() - 1; ++i)
        {
            for (int j = 2; j < mPitch - 1; ++j) {
                size_t ind = (i + 1) * mPitch - j - 1;
                maxRight[ind] = std::max<uint8_t>(maxRight[ind + 1], mTreeImage[i][mPitch - j]);
            }
        }
        return maxRight;
    }

    std::vector<uint8_t> getMaxLeft() {
        size_t treeMapSize = mTreeImage.size() * mPitch;
        std::vector<uint8_t> maxLeft(treeMapSize, 0);
        for (size_t i = 1; i < mPitch - 1; ++i)
        {
            maxLeft[1 + mPitch * i] = mTreeImage[i][0];
        }
        for (size_t i = 1; i < mTreeImage.size() - 1; ++i)
        {
            for (int j = 2; j < mPitch - 1; ++j) {
                size_t ind = i * mPitch + j;
                maxLeft[ind] = std::max<uint8_t>(maxLeft[ind - 1], mTreeImage[i][j - 1]);
            }
        }
        return maxLeft;
    }

    std::vector<uint8_t> getMaxDown() {
        size_t treeMapSize = mTreeImage.size() * mPitch;
        std::vector<uint8_t> maxBottom(treeMapSize, 0);
        for (size_t i = 1; i < mPitch - 1; ++i)
        {
            maxBottom[mPitch + i] = mTreeImage[0][i];
        }
        for (size_t i = 2; i < mTreeImage.size() - 1; ++i)
        {
            for (int j = 1; j < mPitch - 1; ++j) {
                size_t ind = i * mPitch + j;
                maxBottom[ind] = std::max<uint8_t>(maxBottom[ind - mPitch], mTreeImage[i - 1][j]);
            }
        }
        return maxBottom;
    }

    long long int showVisibility() {

        size_t i = 0;
        long long int total = 0;
        for (auto& line : mTreeImage) {
            if (i == 0 || i == mTreeImage.size() - 1) {
                 total += line.size();
            }
            else {
                total += 2;
                for (int j = 1; j < mPitch - 1; ++j) {
                    uint8_t value = line[j];
                    size_t ind = j + i * mPitch;
                    if (value > mMaxTop[ind] || value > mMaxDown[ind] || value > mMaxRight[ind] || value > mMaxLeft[ind])
                        total++;
                }
            }
            i++;
        }
        return total;
    }

    int getRightScore(const int i, const int j, const uint8_t currentValue) {
        int rightScenicScore = 1;
        for (int k = j + 1; k < mPitch - 1; ++k) {
            if (currentValue <= mTreeImage[i][k])
                break;
            rightScenicScore++;
        }
        return rightScenicScore;
    }

    int getLeftScore(const int i, const int j, const uint8_t currentValue) {
        int leftScenicScore = 1;
        for (int k = j - 1; k > 0; --k) {
            if (currentValue <= mTreeImage[i][k])
                break;
            leftScenicScore++;
        }
        return leftScenicScore;
    }

    int getDownScore(const int i, const int j, const uint8_t currentValue) {
        int downScenicScore = 1;
        for (int k = i + 1; k < mTreeImage.size() - 1; ++k) {
            if (currentValue <= mTreeImage[k][j])
                break;
            downScenicScore++;
        }
        return downScenicScore;
    }

    int getUpScore(const int i, const int j, const uint8_t currentValue) {
        int upScenicScore = 1;
        for (int k = i - 1; k > 0; --k) {
            if (currentValue <= mTreeImage[k][j])
                break;
            upScenicScore++;
        }
        return upScenicScore;
    }

    py::array getScenicScoreImage() {

        py::array arr = py::array_t<int>({ mTreeImage.size(), mPitch });
        auto ptr = getPointerToNumpyData<int>(arr);

        size_t i = 0;
        for (auto& line : mTreeImage) {
            if (i == 0 || i == mTreeImage.size() - 1) {
                for (int j = 0; j < mPitch; ++j) {
                    ptr[i * mPitch + j] = 0;
                }
            }
            else {
                ptr[i * mPitch] = 0;
                ptr[(i + 1) * mPitch - 1] = 0;

                for (int j = 1; j < mPitch - 1; ++j) {
                    uint8_t currentValue = line[j];

                    int rightScenicScore = getRightScore(i, j, currentValue);
                    int leftScenicScore = getLeftScore(i, j, currentValue);
                    int upScenicScore = getUpScore(i, j, currentValue);
                    int downScenicScore = getDownScore(i, j, currentValue);

                    ptr[i * mPitch + j] = rightScenicScore * leftScenicScore * upScenicScore * downScenicScore;
                }
            }
            i++;
        }

        return arr;
    }

    std::vector<std::vector<uint8_t>> mTreeImage;
    std::string mFileName;
    size_t mPitch;
    bool mFileParsed = false;

    std::vector<uint8_t> mMaxTop;
    std::vector<uint8_t> mMaxRight;
    std::vector<uint8_t> mMaxLeft;
    std::vector<uint8_t> mMaxDown;
};


void init_day08(py::module& m) {
    py::class_<TreeMap>(m, "TreeMap")
        .def(py::init<std::string&>())
        .def("__bool__", &TreeMap::getFileParsed)
        .def_property_readonly("image", &TreeMap::showImage)
        .def_property_readonly("visibility", &TreeMap::showVisibility)
        .def_property_readonly("up_max", &TreeMap::showUpMax)
        .def_property_readonly("down_max", &TreeMap::showDownMax)
        .def_property_readonly("right_max", &TreeMap::showRightMax)
        .def_property_readonly("left_max", &TreeMap::showLeftMax)
        .def_property_readonly("scenic_scores", &TreeMap::getScenicScoreImage)
        ;
}