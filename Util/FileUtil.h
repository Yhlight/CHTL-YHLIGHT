#pragma once

#include <string>
#include <fstream>
#include <sstream>

class FileUtil {
public:
    static std::string readFile(const std::string& path) {
        std::ifstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
};
