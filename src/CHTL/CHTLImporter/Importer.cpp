#include "Importer.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace CHTL {

std::string Importer::importFile(const std::string& currentPath, const std::string& importPath) {
    std::filesystem::path resolved = resolvePath(currentPath, importPath);
    std::ifstream file(resolved);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + resolved.string());
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::filesystem::path Importer::resolvePath(const std::filesystem::path& currentPath, const std::filesystem::path& importPath) {
    std::filesystem::path currentDir = std::filesystem::path(currentPath).parent_path();
    return std::filesystem::weakly_canonical(currentDir / importPath);
}

} // namespace CHTL
