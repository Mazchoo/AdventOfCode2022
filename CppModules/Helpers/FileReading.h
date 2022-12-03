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
