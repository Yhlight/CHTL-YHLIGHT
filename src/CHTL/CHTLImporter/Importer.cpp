#include "Importer.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"

namespace CHTL {

Importer::Importer() {}

std::unique_ptr<ASTNode> Importer::importFile(const std::string& filePath, const std::string& currentPath) {
    std::filesystem::path path(currentPath);
    path = path.parent_path() / filePath;
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path.string());
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    return parser.parse();
}

std::string Importer::importRawFile(const std::string& filePath, const std::string& currentPath) {
    std::filesystem::path path(currentPath);
    path = path.parent_path() / filePath;
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path.string());
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

}
