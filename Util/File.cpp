#include "File.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace CHTL {
namespace Util {

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Could not open file: " + path);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

} // namespace Util
} // namespace CHTL
