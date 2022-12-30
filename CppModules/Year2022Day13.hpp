
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>

#include "Helpers/FileReading.h"
#include "Helpers/DataStructures.h"

namespace py = pybind11;

typedef BranchNode<uint8_t> distressSignal;
typedef std::pair<distressSignal, distressSignal> distressPairs;


distressSignal parseStrListIntoSignal(std::string& listString) {
    auto node = distressSignal();
    auto currentNode = &node;
    char lastChar = '[';

    for (auto& ch : listString) {
        if (std::isdigit(ch)) {
            if (std::isdigit(lastChar)) {
                uint8_t newValue = currentNode->mChildren.back().mValue;
                newValue *= 10;
                newValue += (uint8_t)ch - 48;
                currentNode->mChildren.back().setValue(newValue);
            }
            else {
                currentNode->appendLeaf((uint8_t)ch - 48);
            } 
        }
        else if (ch == '[') {
            currentNode->appendBranch();
            currentNode = &(currentNode->mChildren.back());
        }
        else if (ch == ']' && currentNode->mParent != nullptr) {
            currentNode = currentNode->mParent;
        }
        lastChar = ch;
    }

    return node;
}


distressSignal initDistressSignal(std::smatch match) {
    distressSignal node;
    int i = 0;

    for (const auto& x : match) {
        if (i == 1) {
            auto listStr = (std::string)x;
            node = parseStrListIntoSignal(listStr);
        }
        i++;
    }
    return node;
};


struct DistressSignalParser {
    DistressSignalParser(std::string& fileName) : mFileName(fileName) {
        std::vector<distressSignal> signals;
        mFileParsed = parseFileLines<BranchNode<uint8_t>>(initDistressSignal, fileName, mLineRegex, signals, 0);
        mFileParsed = mFileParsed && (signals.size() % 2 == 0);

        if (mFileParsed) {
            size_t nrSignals = signals.size();
            mSignals.resize(nrSignals / 2);
            for (size_t i = 0; i < nrSignals; i += 2)
                mSignals[i / 2] = std::make_pair(signals[i], signals[i + 1]);
        }
    }

    py::list parseSignalIntoList(distressSignal signal) {
        py::list output;

        for (auto& node : signal.mChildren) {
            if (node)
                output.append(node.mValue);
            else
                output.append(parseSignalIntoList(node));
        }

        return output;
    }

    std::pair<py::list, py::list> getListSignalPair(size_t i) {
        if (i >= mSignals.size())
            throw  pybind11::index_error("Index out of bounds");

        std::pair<py::list, py::list> output;
        output.first = parseSignalIntoList(mSignals[i].first);
        output.second = parseSignalIntoList(mSignals[i].second);
        return output;
    }

    distressSignal parseList(py::list list) {
        // Probably more efficient by directly indexing the list
        std::string listStr = py::str(list);
        listStr = listStr.substr(1, listStr.size() - 1);
        return parseStrListIntoSignal(listStr);
    }

    bool signalValuesLessThan(distressSignal signal1, distressSignal signal2) {
        return signal1 && signal2 && signal1.mValue < signal2.mValue;
    }

    bool signalValuesGreaterThan(distressSignal signal1, distressSignal signal2) {
        return signal1 && signal2 && signal1.mValue > signal2.mValue;
    }

    bool compareDistressSignals(distressSignal signal1, distressSignal signal2, bool& output) {

        if (signalValuesLessThan(signal1, signal2)) {
            output = true;
            return true;
        }
        else if (signalValuesGreaterThan(signal1, signal2)) {
            output = false;
            return true;
        }
        else if (!signal1 && signal2) {
            auto newSignal = distressSignal();
            newSignal.mChildren.emplace_back(signal2);
            return compareDistressSignals(signal1, newSignal, output);
        }
        else if (signal1 && !signal2) {
            auto newSignal = distressSignal();
            newSignal.mChildren.emplace_back(signal1);
            return compareDistressSignals(newSignal, signal2, output);
        }
        else if (!signal1 && !signal2) {
            size_t minLength = std::min<size_t>(signal1.length(), signal2.length());
            for (size_t i = 0; i < minLength; ++i) {
                if (compareDistressSignals(signal1[i], signal2[i], output))
                    return true;
            }
            if (signal1.length() != signal2.length()) {
                output = signal1.length() == minLength;
                return true;
            }
        }

        return false;
    }

    int getNrListsInRightOrder() {
        int total = 0;
        int i = 0;
        for (auto& pair : mSignals) {
            bool correctOrder = true;
            compareDistressSignals(pair.first, pair.second, correctOrder);
            i++;
            total += (correctOrder) ? i : 0;
        }

        return total;
    }

    bool compareLists(py::list list1, py::list list2) {
        auto signal1 = parseList(list1);
        auto signal2 = parseList(list2);

        bool correctOrder = true;
        compareDistressSignals(signal1, signal2, correctOrder);

        return correctOrder;
    }

    bool getFileParsed() {
        return mFileParsed;
    };

    std::string mFileName;
    bool mFileParsed = false;
    static const std::regex mLineRegex;

    std::vector<distressPairs> mSignals;
};
const std::regex DistressSignalParser::mLineRegex = std::regex("\\[(.*)\\]");


void init_day13(py::module& m) {
    py::class_<DistressSignalParser>(m, "DistressSignalParser")
        .def(py::init<std::string&>())
        .def("__bool__", &DistressSignalParser::getFileParsed)
        .def("__getitem__", [](DistressSignalParser& self, size_t i) {
            return self.getListSignalPair(i);
        })
        .def("compare_lists", &DistressSignalParser::compareLists)
        .def("compare_all_input", &DistressSignalParser::getNrListsInRightOrder)
        ;
}