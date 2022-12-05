
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <regex>

#include "Helpers/FileReading.h"

typedef std::vector<std::deque<char>> stack;
typedef std::list<std::list<char>> pyStack;
typedef std::array<int, 3> move;

namespace py = pybind11;


move initMoveConstruct(std::smatch match) {
    int i = -1;
    move newMove{};

    for (const auto& x : match) {
        if (i != -1) {
            int subtract = (i == 0) ? 0 : 1;
            newMove[i] = std::stoi(x) - subtract;
        } 
        i++;
    }
    return newMove;
};


struct MovingStacks {
    MovingStacks(std::string fileName) : mFileName(fileName) {
        mFileParsed = parseFileLines<move>(initMoveConstruct, fileName, mRegexExp, mMoves, 0);
    }

    bool getFileParsed() {
        return mFileParsed;
    }

    stack getStack() {
        return mStack;
    }

    void setStack(pyStack newStacks) {
        mStack.clear(); // If this is not done then the stack just gets appended each time it is set
        mStack.resize(newStacks.size());
        int i = 0;
        for (auto& stack : newStacks) {
            for (auto& character : stack) {
                mStack[i].emplace_front(character);
            }
            i++;
        }
    }

    stack doAllMoves() {
        for (auto& move : mMoves) {
            auto& sourceStack = mStack[move[1]];
            auto& desintationStack = mStack[move[2]];
            desintationStack.insert(desintationStack.end(), sourceStack.rbegin(), sourceStack.rbegin() + move[0]);
            sourceStack.erase(sourceStack.end() - move[0], sourceStack.end());
        }
        return mStack;
    }

    stack doAllMovesKeepOrder() {
        for (auto& move : mMoves) {
            auto& sourceStack = mStack[move[1]];
            auto& desintationStack = mStack[move[2]];
            desintationStack.insert(desintationStack.end(), sourceStack.end() - move[0], sourceStack.end() );
            sourceStack.erase(sourceStack.end() - move[0], sourceStack.end());
        }
        return mStack;
    }

    stack mStack;
    std::vector<move> mMoves;
    std::string mFileName;
    bool mFileParsed = false;
    static const std::regex mRegexExp;
};
const std::regex MovingStacks::mRegexExp = std::regex("move ([0-9]+) from ([1-9]) to ([1-9])");


void init_day05(py::module& m) {
    py::class_<MovingStacks>(m, "MovingStacks")
        .def(py::init<std::string>())
        .def("__bool__", &MovingStacks::getFileParsed, py::return_value_policy::copy)
        .def("__iter__", [](MovingStacks&self) {
            return py::make_iterator(self.mMoves.begin(), self.mMoves.end());
        })
        .def_property("current_stack", &MovingStacks::getStack, &MovingStacks::setStack)
        .def("do_all_moves", &MovingStacks::doAllMoves)
        .def("do_all_moves_keep_order", &MovingStacks::doAllMovesKeepOrder)
        ;
}