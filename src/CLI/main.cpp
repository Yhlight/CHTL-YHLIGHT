#include "CLILib.h"
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include "CHTL/CHTLAnalyser/Analyser.h"
#include "SharedCore/CHTLContext.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::string read_file_content(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filepath << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void write_file_content(const std::string& filepath, const std::string& content) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filepath << std::endl;
        exit(1);
    }
    file << content;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        CHTL::print_usage();
        return 1;
    }

    CHTL::CommandLineOptions opts = CHTL::parse_arguments(argc, argv);
    std::string source = read_file_content(opts.input_file);

    auto context = std::make_shared<CHTL::CHTLContext>();
    CHTL::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    auto ast = parser.parse();

    std::vector<std::string> importStack;
    CHTL::Analyser analyser(*ast, opts.input_file, importStack, context);
    analyser.analyse();

    CHTL::Generator generator(*ast);
    std::string output = generator.generate();

    if (opts.output_file.empty()) {
        std::cout << output << std::endl;
    } else {
        write_file_content(opts.output_file, output);
    }

    return 0;
}
