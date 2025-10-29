#include "FileUtil.h"
#include <fstream>
#include <sstream>

namespace CHTL {
namespace Util {

std::optional<std::string> FileUtil::readFileContents(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return std::nullopt;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

} // namespace Util
} // namespace CHTL
