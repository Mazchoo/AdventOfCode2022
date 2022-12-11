
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <regex>

#include "Helpers/FileReading.h"
#include "Helpers/NumpyUtils.h"

namespace py = pybind11;

typedef std::pair<std::string, int8_t> cpuOperation;

cpuOperation initCpuOperation(std::smatch match) {
    int i = 0;
    cpuOperation output;

    for (const auto& x : match) {
        if (i == 1)
            output.first = x;
        if (i == 2) {
            if (std::string(x).size() > 1) {
                std::string str = std::string(x);
                output.second = std::stoi(str.substr(1, str.size()));
            }
            else {
                output.second = 0;
            }
        }

        i++;
    }
    return output;
};


struct CathodeRayTube {
    CathodeRayTube(std::string& fileName) : mFileName(fileName) {
        mFileParsed = parseFileLines<cpuOperation>(initCpuOperation, fileName, mRegexExp, mCpuOperations, 0);
        mVectorizedImage.resize(mImgHeight * mImgPitch);
    }

    bool getFileParsed() {
        return mFileParsed;
    };

    int getSignalStrength() {
        return mCycle * mRegisterValue;
    }

    void outputSignalReading() {
        if ((mCycle > 19) && ((mCycle - 20) % 40 == 0) && (mCycle < 221))
            mSignalReadings.emplace_back(getSignalStrength());
    }

    void drawCurrentPixel() {
        mVectorizedImage[mCycle] = std::abs(mRegisterValue - (mCycle % mImgPitch)) < 2;
    }

    void incrementCycle(int8_t incrementAmount) {
        // during
        drawCurrentPixel();
        mCycle++;
        outputSignalReading();
        // after cycle
        mRegisterValue += incrementAmount;
    }

    void addOperation(int8_t amount) {
        incrementCycle(0);
        incrementCycle(amount);
    }

    void processOperation(cpuOperation op) {
        if (op.first == "addx") {
            addOperation(op.second);
        }
        else if (op.first == "noop") {
            incrementCycle(0);
        }
    }

    void resetState() {
        mCycle = 0;
        mRegisterValue = 1;
        mSignalReadings.resize(0);
    }

    void doAllOperations() {
        resetState();
        for (auto& operation : mCpuOperations)
            processOperation(operation);
    }

    long long int getSumOfSignalOperations() {
        long long int output = 0;
        for (auto& value : mSignalReadings)
            output += value;

        return output;
    }

    py::array showCrtImage() {
        py::array arr = py::array_t<bool>({ mImgHeight, mImgPitch });
        auto ptr = getPointerToNumpyData<bool>(arr);
        std::move(mVectorizedImage.begin(), mVectorizedImage.end(), ptr);
        return arr;
    }

    std::vector<cpuOperation> mCpuOperations;
    std::string mFileName;
    bool mFileParsed = false;
    static const std::regex mRegexExp;

    std::list<int> mSignalReadings;
    std::vector<int> mVectorizedImage;
    int mCycle = 0;
    int mRegisterValue = 0;
    const int mImgPitch = 40;
    const int mImgHeight = 6;
};
const std::regex CathodeRayTube::mRegexExp = std::regex("([a-z]+)(.*)");


void init_day10(py::module& m) {
    py::class_<CathodeRayTube>(m, "CathodeRayTube")
        .def(py::init<std::string&>())
        .def("__bool__", &CathodeRayTube::getFileParsed)
        .def("__iter__", [](CathodeRayTube&self) {
            return py::make_iterator(self.mCpuOperations.begin(), self.mCpuOperations.end());
        })
        .def("do_all_ops", &CathodeRayTube::doAllOperations)
        .def_property_readonly("signal_sum", &CathodeRayTube::getSumOfSignalOperations)
        .def_property_readonly("image", &CathodeRayTube::showCrtImage)
        ;
}