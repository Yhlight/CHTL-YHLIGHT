#pragma once

#include <string>
#include <filesystem>

class Importer {
public:
    std::string read(const std::filesystem::path& path);
};
