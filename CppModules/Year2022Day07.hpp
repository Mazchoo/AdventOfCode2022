
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <memory>

#include "Helpers/FileReading.h"
#define INTEGER_LIMIT 1000000000000

namespace py = pybind11;

struct Directory {
    std::string mName;
    std::list<std::unique_ptr<Directory>> mChildren = {};
    Directory* mParent = nullptr;
    bool mIsFile = false;
    long long int mMemory = 0;

    Directory() { mName = "root";  }
    Directory(std::string name, Directory* parent) : mName(name), mParent(parent) {}
    Directory(std::string name, Directory* parent, long long int memory) :
        mName(name), mParent(parent), mMemory(memory)
    {
        mIsFile = true;
    }
};


Directory* changeCurrentDirectory(std::string& line, Directory* currentDir) {
    std::string name = line.substr(5, line.size());
    Directory* foundDir = nullptr;
    for (auto& child : currentDir->mChildren) {
        if (child->mName == name) {
            foundDir = child.get();
            break;
        }
    }
    return foundDir;
}


void addFileToDirectory(std::string line, const std::regex& fileRegex, Directory* currentDir) {
    std::smatch match;
    std::regex_search(line, match, fileRegex);

    long long int memorySize;
    std::string name;

    int i = 0;
    for (const auto& x : match) {
        if (i == 1) {
            memorySize = std::stoi(x);
        }
        else if (i == 2) {
            name = x;
        }
        i++;
    }

    currentDir->mChildren.emplace_back(std::make_unique<Directory>(name, currentDir, memorySize));
}


struct FileCommands {
    FileCommands(std::string fileName) : mFileName(fileName) {
        mFileParsed = parseFileLines<std::string>(initLine, fileName, mRegexExp, mCommands, 0);
        mFileTreeParsed = parseFileTree();
    }

    bool getFileParsed() {
        return mFileTreeParsed;
    };

    bool parseFileTree() {
        if (!mFileParsed)
            return false;
        
        mDirectory = std::make_unique<Directory>();
        Directory* currentDir = nullptr;
        for (auto& line : mCommands) {
            if (line.substr(0, 6) == "$ cd /") {
                currentDir = mDirectory.get();
            }
            else if (line.substr(0, 7) == "$ cd ..") {
                if (currentDir->mParent == nullptr)
                    return false;
                currentDir = currentDir->mParent;
            }
            else if (line.substr(0, 4) == "dir ") {
                currentDir->mChildren.emplace_back(std::make_unique<Directory>(line.substr(4, line.size()), currentDir));
            }
            else if (line.substr(0, 4) == "$ cd") {
                currentDir = changeCurrentDirectory(line, currentDir);
                if (currentDir == nullptr)
                    return currentDir;
            }
            else if (std::regex_match(line, mFileRegex)) {
                addFileToDirectory(line, mFileRegex, currentDir);
            }
        }

        return true;
    }

    long long int calculateFolderSizes(Directory* dir) {
        if (dir->mIsFile)
            return dir->mMemory;
        long long int total = 0;

        for (auto& child : dir->mChildren)
            total += calculateFolderSizes(child.get());

        dir->mMemory = total;
        return total;
    }

    long long int getTotalFileSize() {
        if (!mFileTreeParsed)
            return 0;

        mMemoryCalculated = true;
        return calculateFolderSizes(mDirectory.get());
    }

    long long int getFolderTotalBelowThreshold(Directory* dir, const long long int threshold) {
        if (dir->mIsFile)
            return 0;

        long long int total = 0;
        if (dir->mMemory <= threshold)
            total += dir->mMemory;

        for (auto& child : dir->mChildren)
            total += getFolderTotalBelowThreshold(child.get(), threshold);

        return total;
    }

    long long int getTotalOfAllDirectoriesBelowThreshold(const long long int threshold) {
        if (!mFileTreeParsed)
            return 0;

        if (!mMemoryCalculated) {
            calculateFolderSizes(mDirectory.get());
            mMemoryCalculated = true;
        }

        return getFolderTotalBelowThreshold(mDirectory.get(), threshold);
    }

    void getSmallestDirectoryAboveThresh(Directory* dir, long long int& margin, const long long int threshold) {
        if (dir->mIsFile)
            return;

        if (dir->mMemory >= threshold) {
            margin = std::min<long long int>(margin, dir->mMemory - threshold);
        }
        else {
            return;
        }
 
        for (auto& child : dir->mChildren)
            getSmallestDirectoryAboveThresh(child.get(), margin, threshold);
    }

    long long int getDirectorySuitableForDeletion(const long long int threshold) {
        if (!mFileTreeParsed)
            return 0;

        if (!mMemoryCalculated) {
            calculateFolderSizes(mDirectory.get());
            mMemoryCalculated = true;
        }

        long long int marginAbove = INTEGER_LIMIT;
        getSmallestDirectoryAboveThresh(mDirectory.get(), marginAbove, threshold);
        if (marginAbove == INTEGER_LIMIT)
            marginAbove = -1;

        return threshold + marginAbove;
    }

    std::vector<std::string> mCommands;
    std::string mFileName;
    bool mFileParsed = false;
    bool mFileTreeParsed = false;
    bool mMemoryCalculated = false;
    static const std::regex mRegexExp;
    static const std::regex mFileRegex;
    std::unique_ptr<Directory> mDirectory = nullptr;
};
const std::regex FileCommands::mRegexExp = std::regex("(.+)");
const std::regex FileCommands::mFileRegex = std::regex("([0-9]+)\\s([a-z\\.]+)");


void init_day07(py::module& m) {
    py::class_<FileCommands>(m, "FileCommands")
        .def(py::init<std::string&>())
        .def("__bool__", &FileCommands::getFileParsed)
        .def("__iter__", [](FileCommands&self) {
            return py::make_iterator(self.mCommands.begin(), self.mCommands.end());
        })
        .def_property_readonly("total_size", &FileCommands::getTotalFileSize)
        .def("total_below_thresh", &FileCommands::getTotalOfAllDirectoriesBelowThreshold)
        .def("dir_to_delete", &FileCommands::getDirectorySuitableForDeletion)
        ;
}