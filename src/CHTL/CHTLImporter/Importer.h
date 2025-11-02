#pragma once

#include <string>
#include <filesystem>

namespace CHTL {

class Importer {
public:
    std::string importFile(const std::string& currentPath, const std::string& importPath);

private:
    std::filesystem::path resolvePath(const std::filesystem::path& currentPath, const std::filesystem::path& importPath);
};

} // namespace CHTL
