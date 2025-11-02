#include "Test.h"
#include <stdexcept>
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLGenerator/Generator.h"

#define ASSERT(condition) \
    if (!(condition)) { \
        throw std::runtime_error("Assertion failed at " + std::string(__FILE__) + ":" + std::to_string(__LINE__)); \
    }

void GeneratorTests() {
    tests.push_back({"Test Basic Generation", []() {
        std::string source = R"(
            div {
                id: "main";
                text { "Hello, CHTL!" }
            }
        )";

        Lexer lexer(source);
        Parser parser(lexer.tokenize());
        Generator generator(parser.parse());
        std::string result = generator.generate();

        std::string expected = R"(<div id="main">Hello, CHTL!</div>)";
        ASSERT(result == expected);
    }});
}
