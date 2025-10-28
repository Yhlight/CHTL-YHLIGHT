#pragma once

#include <string>
#include <vector>

namespace CHTL {
namespace Util {

// Trims whitespace from both ends of a string.
std::string Trim(const std::string& str);

// Splits a string into a vector of substrings based on a delimiter.
std::vector<std::string> Split(const std::string& str, char delimiter);

} // namespace Util
} // namespace CHTL
