#pragma once
#define MAX_PATH          260
#define BUILD_FOLDER      "build\\ExeDebugBuild\\"

#include <windows.h>
#include <string>

const std::string buildFolder = BUILD_FOLDER;

void removeBuildDirFromRoot(std::string& str) {
    size_t outputStrLen = str.length();
    if (outputStrLen > buildFolder.length()) {
        std::string endString = str.substr(outputStrLen - buildFolder.length(), outputStrLen);
        if (endString == BUILD_FOLDER) {
            str = str.substr(0, outputStrLen - buildFolder.length());
        }
    }
}

std::string RootFolder() {
    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    int offset = 0;
    int i = 0;
    for (auto& chr : buffer) {
        i++;
        if (chr == '/' or chr == '\\')
            offset = i;
    }
    std::string output_string(buffer, buffer + offset);

    removeBuildDirFromRoot(output_string);

    return output_string;
}


template <typename T> bool parseFileLines(const std::function<T(std::smatch)>& constructor, std::string& fileName,
                                          const std::regex& regexExp, std::vector<T>& outputVec, const int verbosity)
{
    std::fstream file;
    file.open(fileName, std::ios::in);
    if (file.is_open()) {
        std::string line;
        std::list<T> objList;

        while (getline(file, line)) {
            if (std::regex_match(line, regexExp)) {
                std::smatch match;
                std::regex_search(line, match, regexExp);
                objList.emplace_back(constructor(match));
            }
            else {
                if (verbosity > 0)
                    std::cout << "String does not conform to expected regex : " << line << std::endl;
            }
        }

        file.close();
        outputVec = std::vector<T>(objList.begin(), objList.end());
        return true;
    }
    return false;
}
