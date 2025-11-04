#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include "CHTL/CHTLNode/ASTNode.h"

void show_usage() {
    std::cout << "Usage: chtl <input_file> [--default-struct] [--inline]" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        show_usage();
        return 1;
    }

    std::string input_file;
    bool default_struct = false;
    bool inline_output = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--default-struct") {
            default_struct = true;
        } else if (arg == "--inline") {
            inline_output = true;
        } else {
            input_file = arg;
        }
    }

    if (input_file.empty()) {
        show_usage();
        return 1;
    }

    std::ifstream file(input_file);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << input_file << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();

    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    CHTL::Generator generator(*ast);
    std::string output = generator.generate(default_struct, inline_output);

    std::cout << output << std::endl;

    return 0;
}
