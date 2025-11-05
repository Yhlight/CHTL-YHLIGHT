#pragma once

#include <string>
#include <fstream>
#include <sstream>

namespace FileUtil {
    inline std::string read_file(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}
