#include "Importer.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLNode/ASTNode.h"

namespace CHTL {

Importer::Importer() {}

std::unique_ptr<ASTNode> Importer::importFile(const std::string& filePath, const std::string& currentPath) {
    std::filesystem::path path(currentPath);
    path = path.parent_path() / filePath;

    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("File not found for import: " + path.string());
    }

    std::string extension = path.extension().string();

    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path.string());
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    if (extension == ".chtl") {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();
        Parser parser(tokens, source);
        return parser.parse();
    } else if (extension == ".html" || extension == ".css" || extension == ".js") {
        auto originNode = std::make_unique<OriginNode>();
        originNode->content = source;
        if (extension == ".html") {
            originNode->type = "Html";
        } else if (extension == ".css") {
            originNode->type = "Style";
        } else if (extension == ".js") {
            originNode->type = "JavaScript";
        }
        return originNode;
    }

    // Fallback for files with no extension or unknown extension
	Lexer lexer(source);
	auto tokens = lexer.tokenize();
	Parser parser(tokens, source);
	return parser.parse();
}

}
