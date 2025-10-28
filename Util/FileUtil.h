#pragma once

#include <string>
#include <vector>

namespace CHTL {

class FileUtil {
public:
    static std::string readFile(const std::string& path);
};

} // namespace CHTL
