#include "Importer.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::string Importer::read(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path.string());
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
