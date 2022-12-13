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


std::string initLine(std::smatch match) {
    bool firstLine = true;
    std::string output;

    for (const auto& x : match) {
        if (firstLine) {
            firstLine = false;
        }
        else {
            output = x;
        }
    }
    return output;
};


std::list<std::string> splitByWord(std::string text, std::string split_word) {
    std::list<std::string> list;
    std::string word = "";
    int is_word_over = 0;

    for (int i = 0; i <= text.length(); i++) {
        if (i <= text.length() - split_word.length()) {
            if (text.substr(i, split_word.length()) == split_word) {
                list.insert(list.end(), word);
                word = "";
                is_word_over = 1;
            }
            //now we want that it jumps the rest of the split character
            else if (is_word_over >= 1) {
                if (is_word_over != split_word.length()) {
                    is_word_over += 1;
                    continue;
                }
                else {
                    word += text[i];
                    is_word_over = 0;
                }
            }
            else {
                word += text[i];
            }
        }
        else {
            word += text[i];
        }
    }
    list.insert(list.end(), word);
    return list;
}
