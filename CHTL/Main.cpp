#include <iostream>
#include <fstream>
#include <sstream>
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLGenerator/Generator.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
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

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    std::unique_ptr<ASTNode> ast = parser.parse();

    if (ast) {
        Generator generator;
        std::string html = generator.generate(*ast);
        std::cout << html << std::endl;
    } else {
        std::cerr << "Error: Parsing failed." << std::endl;
        return 1;
    }

    return 0;
}
