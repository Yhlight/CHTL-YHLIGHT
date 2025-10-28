
#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLGenerator/Generator.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    std::shared_ptr<CHTL::BaseNode> root = parser.parse();

    CHTL::Generator generator(root, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates(), parser.getOriginBlocks());
    std::string result = generator.generate();

    std::cout << result << std::endl;

    return 0;
}
