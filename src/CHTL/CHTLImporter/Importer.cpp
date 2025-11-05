#include "Importer.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace CHTL {

std::string Importer::importFile(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("File not found: " + path.string());
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path.string());
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

} // namespace CHTL
