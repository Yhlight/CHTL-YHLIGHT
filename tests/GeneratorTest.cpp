#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"

using namespace CHTL;

// Helper function to remove whitespace for easier comparison
inline std::string removeWhitespace(std::string str) {
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
    return str;
}


TEST(GeneratorTest, GeneratesHtmlFromAst) {
    std::string source = R"(
        html {
            body {
                div {
                    id: "main-content";
                    class: "container";
                }
                p {
                    text { "Hello, CHTL!" }
                }
            }
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer.tokenize(), source);
    auto program = parser.parse();
    Generator generator(*program);
    std::string result = generator.generate();

    std::string expected = R"(
        <html>
          <body>
            <div id="main-content" class="container"></div>
            <p>
              Hello, CHTL!
            </p>
          </body>
        </html>
    )";

    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}
