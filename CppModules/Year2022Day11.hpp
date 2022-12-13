
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>

#include "Helpers/FileReading.h"

namespace py = pybind11;

enum class FunctionType {
    Add,
    Multiply,
    Square
};

typedef std::pair<FunctionType, int> monkeyOperation;
typedef std::pair<std::string, int> pyOperation;


std::list<unsigned long long int> initItems(std::smatch match) {
    int i = 0;
    std::list<unsigned long long int> output;

    for (const auto& x : match) {
        if (i == 1) {
            std::list<std::string> items = splitByWord(x, ",");
            for (auto& item: items)
                output.emplace_back(std::stoi(item));
        }

        i++;
    }
    return output;
};


int parseInt(std::smatch match) {
    int i = 0;
    int output = 0;

    for (const auto& x : match) {
        if (i == 1)
            output = std::stoi(x);
        i++;
    }
    return output;
};


monkeyOperation initMonkeyOp(std::smatch match) {
    int i = 0;
    monkeyOperation output;

    for (const auto& x : match) {
        if (i == 1) {
            if (x == "*")
                output.first = FunctionType::Multiply;
            else if (x == "+")
                output.first = FunctionType::Add;
        }
        if (i == 2) {
            if (x == "old")
                output = std::make_pair(FunctionType::Square, 0); 
            else
                output.second = std::stoi(x);
        }

        i++;
    }
    return output;
};


struct Monkey {
    Monkey(std::list<unsigned long long int>& items, int test, int trueInd, int falseInd, monkeyOperation op, bool worry) :
           mTestModulo(test), mTrueInd(trueInd), mFalseInd(falseInd), mWorryReduce(worry), mOperation(op), mItems(items) {}

    unsigned long long int doOperation(unsigned long long int itemValue) {
        if (mOperation.first == FunctionType::Add)
            itemValue += mOperation.second;
        else if (mOperation.first == FunctionType::Multiply)
            itemValue *= mOperation.second;
        else if (mOperation.first == FunctionType::Square)
            itemValue *= itemValue;

        return itemValue;
    }

    bool inspectItem(unsigned long long int &itemValue, size_t &monkeyInd, const unsigned long long int modulusProduct) {
        if (mItems.empty())
            return false;

        itemValue = mItems.front();
        mItems.pop_front();
        itemValue = doOperation(itemValue);
        if (mWorryReduce)
            itemValue /= 3;

        if (itemValue > modulusProduct)
            itemValue %= modulusProduct;

        if (itemValue % mTestModulo == 0)
            monkeyInd = mTrueInd;
        else
            monkeyInd = mFalseInd;

        nrItemsInspected++;
        return true;
    }

    uint32_t nrItemsInspected = 0;

    int mTestModulo;
    size_t mTrueInd;
    size_t mFalseInd;
    bool mWorryReduce;
    monkeyOperation mOperation;
    std::list<unsigned long long int> mItems;
};


struct MonkeyMadness {
    MonkeyMadness(std::string& fileName, bool worryReduce = true) : mFileName(fileName) {
        InitializeMonkeys(fileName, worryReduce);

        mModulusProduct = 1;
        for (auto& monkey : mMonkeys)
            mModulusProduct *= monkey.mTestModulo;
    }

    void InitializeMonkeys(std::string& fileName, bool worryReduce) {
        std::vector<std::list<unsigned long long int>> items;
        mFileParsed = parseFileLines<std::list<unsigned long long int>>(initItems, fileName, mItemRegex, items, 0);

        std::vector<int> tests;
        mFileParsed = mFileParsed && parseFileLines<int>(parseInt, fileName, mTestRegex, tests, 0);

        std::vector<int> trueInds;
        mFileParsed = mFileParsed && parseFileLines<int>(parseInt, fileName, mTrueRegex, trueInds, 0);

        std::vector<int> falseInds;
        mFileParsed = mFileParsed && parseFileLines<int>(parseInt, fileName, mFalseRegex, falseInds, 0);

        std::vector<monkeyOperation> monkeyOps;
        mFileParsed = mFileParsed && parseFileLines<monkeyOperation>(initMonkeyOp, fileName, mOperationRegex, monkeyOps, 0);

        mFileParsed = mFileParsed && (items.size() == tests.size());
        mFileParsed = mFileParsed && (tests.size() == trueInds.size());
        mFileParsed = mFileParsed && (trueInds.size() == falseInds.size());
        mFileParsed = mFileParsed && (falseInds.size() == monkeyOps.size());

        if (mFileParsed) {
            size_t nrMonkeys = items.size();
            mMonkeys.reserve(nrMonkeys);
            for (size_t i = 0; i < nrMonkeys; ++i)
                mMonkeys.emplace_back(Monkey(items[i], tests[i], trueInds[i], falseInds[i], monkeyOps[i], worryReduce));
        }
    }

    bool getFileParsed() {
        return mFileParsed;
    };

    void processMonkey(Monkey& monkey) {
        unsigned long long int itemValue;
        size_t monkeyInd;
        while (monkey.inspectItem(itemValue, monkeyInd, mModulusProduct))
            mMonkeys[monkeyInd].mItems.emplace_back(itemValue);
    }

    void doMonkeyRounds(uint32_t nrRounds) {
        for (uint32_t i = 0; i < nrRounds; ++i) {
            for (auto& monkey : mMonkeys)
                processMonkey(monkey);
        }
    }

    pyOperation getPyOp(monkeyOperation& monkeyOp) {
        std::string functionStr;

        if (monkeyOp.first == FunctionType::Add)
            functionStr = "add";
        else if (monkeyOp.first == FunctionType::Multiply)
            functionStr = "multiply";
        else if (monkeyOp.first == FunctionType::Square)
            functionStr = "square";

        return std::make_pair(functionStr, monkeyOp.second);
    }

    py::dict parseMonkey(size_t monkeyInd) {
        if (monkeyInd >= mMonkeys.size())
            throw  pybind11::index_error("Index out of bounds");

        py::dict output;
        Monkey monkey = mMonkeys[monkeyInd];

        output["test"] = monkey.mTestModulo;
        output["true"] = monkey.mTrueInd;
        output["false"] = monkey.mFalseInd;
        output["items"] = monkey.mItems;
        output["func"] = getPyOp(monkey.mOperation);

        return output;
    }

    std::vector<uint32_t> getItemInspections() {
        std::vector<uint32_t> output;
        output.resize(mMonkeys.size());

        int i = 0;
        for (auto& monkey : mMonkeys)
            output[i++] = monkey.nrItemsInspected;

        std::sort(output.begin(), output.end());

        return output;
    }

    std::vector<Monkey> mMonkeys;
    std::string mFileName;
    bool mFileParsed = false;
    unsigned long long int mModulusProduct;

    static const std::regex mItemRegex;
    static const std::regex mTestRegex;
    static const std::regex mTrueRegex;
    static const std::regex mFalseRegex;
    static const std::regex mOperationRegex;
};
const std::regex MonkeyMadness::mItemRegex = std::regex("  Starting items: (.*)");
const std::regex MonkeyMadness::mTestRegex = std::regex("  Test: divisible by (\\d+)");
const std::regex MonkeyMadness::mTrueRegex = std::regex("    If true: throw to monkey (\\d+)");
const std::regex MonkeyMadness::mFalseRegex = std::regex("    If false: throw to monkey (\\d+)");
const std::regex MonkeyMadness::mOperationRegex = std::regex("  Operation: new = old ([\\*\\+]) (.*)");


void init_day11(py::module& m) {
    py::class_<MonkeyMadness>(m, "MonkeyMadness")
        .def(py::init<std::string&>())
        .def(py::init<std::string&, bool>())
        .def("__bool__", &MonkeyMadness::getFileParsed)
        .def("__getitem__", [](MonkeyMadness&self, int i) {
            return self.parseMonkey(i);
        })
        .def("do_rounds", &MonkeyMadness::doMonkeyRounds)
        .def_property_readonly("inspections", &MonkeyMadness::getItemInspections)
        ;
}