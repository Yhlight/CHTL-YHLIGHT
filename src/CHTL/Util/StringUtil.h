#pragma once

#include <string>
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

} // namespace CHTL
