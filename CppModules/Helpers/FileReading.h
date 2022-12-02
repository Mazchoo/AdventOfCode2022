#pragma once
#define MAX_PATH          260

#include <windows.h>
#include <string>

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

    size_t endString = 0;
    bool finished = false;
    for (size_t i = output_string.size(); i > 0; --i)
    {
        if (output_string[i] == '\\')
        {
            if (endString != 0)
                finished = true;
            endString = i;
        }
        if (finished)
            break;
    }
    output_string = output_string.substr(0, endString);

    return output_string;
}
