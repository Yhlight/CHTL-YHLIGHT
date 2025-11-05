#pragma once

#include <string>
#include <filesystem>

namespace CHTL {

class Importer {
public:
    std::string importFile(const std::filesystem::path& path);
};

} // namespace CHTL
