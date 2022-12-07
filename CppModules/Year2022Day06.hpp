
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

#include "Helpers/FileReading.h"

namespace py = pybind11;


struct SignalPacket {
    SignalPacket(std::string fileName) : mFileName(fileName) {
        mFileParsed = parseFileLines<std::string>(initLine, fileName, mRegexExp, mPackets, 0);
    }

    bool getFileParsed() {
        return mFileParsed;
    };
    

    std::vector<int> getDistinctChars(int nrChars) {
        std::vector<int> output;
        output.resize(mPackets.size());
        int i = 0;

        for (auto& packet : mPackets) {
            std::vector<char> currentArray;
            currentArray.resize(nrChars);
            for (int j = 0; j < nrChars; ++j)
                currentArray[j] = packet[j];

            for (int charPosition = nrChars; charPosition < packet.length(); ++charPosition) {
                if (charsAreDistinct(currentArray)) {
                    output[i] = charPosition;
                    break;
                }

                for (int j = 0; j < nrChars - 1; ++j) {
                    currentArray[j] = currentArray[(size_t)j + 1];
                }
                currentArray[(size_t)nrChars - 1] = packet[charPosition];
            }
            i++;
        }

        return output;
    }

    bool charsAreDistinct(std::vector<char>& arr) {
        for (int i = 0; i < arr.size(); ++i) {
            for (int j = i + 1; j < arr.size(); ++j) {
                if (arr[i] == arr[j])
                    return false;
            }
        }
        return true;
    }

        std::vector<int> getEndOfFirstFourDistinctChars() {
        std::vector<int> output;
        output.resize(mPackets.size());
        int i = 0;

        for (auto& packet : mPackets) {
            std::array<char, 4> currentArray = { packet[0], packet[1], packet[2], packet[3] };

            for (int charPosition = 4; charPosition < packet.length(); ++charPosition) {
                if (fourCharsAreDistinct(currentArray)) {
                    output[i] = charPosition;
                    break;
                }

                for (int j = 0; j < 3; ++j) {
                    currentArray[j] = currentArray[(size_t)j + 1];
                }
                currentArray[3] = packet[charPosition];
            }
            i++;
        }

        return output;
    }

    bool fourCharsAreDistinct(std::array<char, 4>& arr) {
        for (int i = 0; i < 4; ++i) {
            for (int j = i + 1; j < 4; ++j) {
                if (arr[i] == arr[j])
                    return false;
            }
        }
        return true;
    }

    std::vector<std::string> mPackets;
    std::string mFileName;
    bool mFileParsed = false;
    static const std::regex mRegexExp;
};
const std::regex SignalPacket::mRegexExp = std::regex("(.+)");


void init_day06(py::module& m) {
    py::class_<SignalPacket>(m, "SignalPacket")
        .def(py::init<std::string>())
        .def("__bool__", &SignalPacket::getFileParsed, py::return_value_policy::copy)
        .def("__iter__", [](SignalPacket&self) {
            return py::make_iterator(self.mPackets.begin(), self.mPackets.end());
        })
        .def("get_packet_starts", &SignalPacket::getDistinctChars)
        ;
}