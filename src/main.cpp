#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include "CHTL/CHTLAnalyser/Analyser.h"

using namespace CHTL;

int main() {
    std::string source = R"(
        html {
            head {
                title { text { "My CHTL Page" } }
            }
            body {
                h1 {
                    id: "main-heading";
                    style {
                        color: blue;
                        text-align: center;
                    }
                    text { "Welcome to CHTL!" }
                }
                p {
                    class: "intro";
                    text { "This is a paragraph generated from CHTL." }
                }
                div {
                    class: "box";
                    style {
                        width: 100px;
                    }
                    text { "This is a box." }
                }
                div {
                    class: "calculated";
                    style {
                        width: 100px * 2 + 50px;
                        height: 50 * 2;
                        border: 1px solid black;
                    }
                    text { "This div has a calculated width." }
                }
                div {
                    class: "referenced";
                    style {
                        width: .box.width + 25px;
                        height: 50px;
                        border: 1px solid red;
                    }
                    text { "This div has a referenced width." }
                }
            }
        }
    )";

    try {
        // 1. Lexer: Tokenize the source code
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();

        // 2. Parser: Build the Abstract Syntax Tree (AST)
        Parser parser(tokens);
        std::unique_ptr<ProgramNode> ast = parser.parse();

        // 3. Analyser: Perform semantic analysis and resolve references
        Analyser analyser(*ast, "main.chtl");
        analyser.analyse();

        // 4. Generator: Generate HTML from the AST
        Generator generator(*ast);
        std::string html_output = generator.generate();

        // 5. Print the result
        std::cout << "--- CHTL Source ---" << std::endl;
        std::cout << source << std::endl;
        std::cout << "\n--- Generated HTML ---" << std::endl;
        std::cout << html_output << std::endl;

    } catch (const std::runtime_error& e) {
        std::cerr << "Compilation Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
