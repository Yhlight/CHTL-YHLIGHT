#pragma once

#include <string>
#include <optional>

namespace CHTL {
namespace Util {

class FileUtil {
public:
    static std::optional<std::string> readFileContents(const std::string& filepath);
};

} // namespace Util
} // namespace CHTL
