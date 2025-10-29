#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

namespace CHTL {

inline std::string formatDouble(double val) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(10) << val;
    std::string s = ss.str();
    s.erase(s.find_last_not_of('0') + 1, std::string::npos);
    s.erase(s.find_last_not_of('.') + 1, std::string::npos);
    return s;
}

inline std::string join(const std::vector<std::string>& elements, const std::string& delimiter) {
    if (elements.empty()) {
        return "";
    }
    std::stringstream ss;
    ss << elements[0];
    for (size_t i = 1; i < elements.size(); ++i) {
        ss << delimiter << elements[i];
    }
    return ss.str();
}

} // namespace CHTL
