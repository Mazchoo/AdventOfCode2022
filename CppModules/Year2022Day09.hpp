
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

typedef std::pair<char, uint8_t> bridgeMove;
typedef std::array<int, 2> point2D;

bridgeMove initBridgeMove(std::smatch match) {
    int i = 0;
    bridgeMove output;

    for (const auto& x : match) {
        if (i == 1)
            output.first = std::string(x)[0];
        if (i == 2)
            output.second = std::stoi(x);
        i++;
    }
    return output;
};


struct RopeBridge {
    RopeBridge(std::string& fileName, uint32_t chainLength = 2) : mFileName(fileName), mChainLength(chainLength) {
        mFileParsed = parseFileLines<bridgeMove>(initBridgeMove, fileName, mRegexExp, mBridgeMoves, 0);
        initializeRopeChain();
    }

    void initializeRopeChain() {
        mRopeChain.resize(mChainLength);

        for (auto& knot : mRopeChain)
            knot = { 0, 0 };
    }

    bool getFileParsed() {
        return mFileParsed;
    };

    int pointMaxAbsDistance(point2D &p1, point2D &p2) {
        return std::max<char>(std::abs(p1[0] - p2[0]), std::abs(p1[1] - p2[1]));
    }

    void modifyCoordinate(point2D &coordinate, const char direction) {
        if (direction == 'U') {
            coordinate[0]++;
        }
        else if (direction == 'D') {
            coordinate[0]--;
        }
        else if (direction == 'R') {
            coordinate[1]++;
        }
        else if (direction == 'L') {
            coordinate[1]--;
        }
        else {
            std::cout << "Invalid move <= " << direction << std::endl;
        }
    }

    void doMove(bridgeMove move) {
        for (uint8_t i = 0; i < move.second; ++i) {
            modifyCoordinate(mRopeChain[0], move.first);

            for (size_t j = 1; j < mChainLength; j++) {
                int dY = mRopeChain[j - 1][0] - mRopeChain[j][0];
                int dX = mRopeChain[j - 1][1] - mRopeChain[j][1];

                if (pointMaxAbsDistance(mRopeChain[j - 1], mRopeChain[j]) > 1) {
                    if (dX == 0) {
                        mRopeChain[j][0] += dY / 2;
                    }
                    else if (dY == 0) {
                        mRopeChain[j][1] += dX / 2;
                    }
                    else {
                        mRopeChain[j][0] += (dY > 0) ? 1 : -1;
                        mRopeChain[j][1] += (dX > 0) ? 1 : -1;
                    }
                }
            }
            mTailVisits.insert(mRopeChain[mChainLength - 1]);
        }
    }

    size_t doAllMoves() {
        if (!mTailVisits.empty())
            return mTailVisits.size();

        for (auto& move : mBridgeMoves)
            doMove(move);

        return mTailVisits.size();
    }

    std::pair<point2D, point2D> getMinMaxCordinates(std::vector<point2D> &vec) {
        point2D minCoord = { 0, 0 };
        point2D maxCoord = { 0, 0 };

        for (auto& coord : vec) {
            minCoord[0] = std::min<int>(minCoord[0], coord[0]);
            minCoord[1] = std::min<int>(minCoord[1], coord[1]);
            maxCoord[0] = std::max<int>(maxCoord[0], coord[0]);
            maxCoord[1] = std::max<int>(maxCoord[1], coord[1]);
        }

        return std::make_pair(minCoord, maxCoord);
    }

    py::array convertChainIntoImage(std::pair<point2D, point2D>& minMaxCoord, std::vector<point2D>& chain) {
        size_t height = (size_t)minMaxCoord.second[0] - minMaxCoord.first[0] + 1;
        size_t width = (size_t)minMaxCoord.second[1] - minMaxCoord.first[1] + 1;
        size_t image_size = height * width;

        py::array arr = py::array_t<bool>({ height, width });
        auto ptr = getPointerToNumpyData<bool>(arr);

        for (size_t i = 0; i < image_size; ++i)
            ptr[i] = false;

        // by convention image has inverted y
        for (auto& visit : chain) {
            int visit_height = visit[0] - minMaxCoord.first[0] + 1;
            int visit_width = visit[1] - minMaxCoord.first[1];
            ptr[image_size - visit_height * width + visit_width] = true;
        }
        return arr;
    }

    py::array showTailVisits() {
        auto setVector = std::vector<point2D>(mTailVisits.begin(), mTailVisits.end());
        std::pair<point2D, point2D> minMaxCoord = getMinMaxCordinates(setVector);
        return convertChainIntoImage(minMaxCoord, setVector);
    }

    py::array showChain() {
        std::pair<point2D, point2D> minMaxCoord = getMinMaxCordinates(mRopeChain);
        return convertChainIntoImage(minMaxCoord, mRopeChain);
    }

    std::vector<bridgeMove> mBridgeMoves;
    std::string mFileName;
    bool mFileParsed = false;
    static const std::regex mRegexExp;
    std::set<point2D> mTailVisits;
    uint32_t mChainLength;
    std::vector<point2D> mRopeChain;
};
const std::regex RopeBridge::mRegexExp = std::regex("([URLD]) ([0-9]+)");


void init_day09(py::module& m) {
    py::class_<RopeBridge>(m, "RopeBridge")
        .def(py::init<std::string&, int>())
        .def(py::init<std::string&>())
        .def("__bool__", &RopeBridge::getFileParsed)
        .def("__iter__", [](RopeBridge&self) {
            return py::make_iterator(self.mBridgeMoves.begin(), self.mBridgeMoves.end());
        })
        .def("do_all_moves", &RopeBridge::doAllMoves)
        .def("do_move", &RopeBridge::doMove)
        .def_property_readonly("tail_move_map", &RopeBridge::showTailVisits)
        .def_property_readonly("chain", &RopeBridge::showChain)
        ;
}