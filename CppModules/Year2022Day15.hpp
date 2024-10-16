
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

struct SensorReading {
    int sensorX;
    int sensorY;
    int beaconX;
    int beaconY;
}


struct ParticleFilter {
    ParticleFilter(std::string& fileName, size_t startX, size_t startY) :
        mFileName(fileName), mStartX(startX), mStartY(startY) {
        mFileParsed = parseFileLines<SensorReading>(initLedgeVector, fileName, mLineRegex, mLedges, 0);
    }

    bool getFileParsed() const {
        return mFileParsed;
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

    std::string mFileName;
    bool mFileParsed = false;
    static const std::regex mLineRegex;

    size_t mWidth = 0;
    size_t mHeight = 0;
    size_t mMinX = 0;
    size_t mMinY = 0;
};
const std::regex ParticleFilter::mLineRegex = std::regex(".*=/d.*=/d.*=/d.*=/d");


void init_day15(py::module& m) {
    py::class_<ParticleFilter>(m, "ParticleFilter")
        .def(py::init<std::string&, int, int>())
        .def("__bool__", &ParticleFilter::getFileParsed)
        .def_property_readonly("scans", &ParticleFilter::scans)
        ;
}