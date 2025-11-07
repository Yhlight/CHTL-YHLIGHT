#include "gtest/gtest.h"
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLGenerator/Generator.h"
#include "CHTLNode/ProgramNode.h"
#include <memory>

TEST(CustomElementTemplateTest, SimpleSpecialization) {
    std::string source = R"(
        [Custom] @Element Box {
            div {
                style {
                    width: 100px;
                }
            }
        }

        body {
            @Element Box {
                div {
                    style {
                        height: 200px;
                    }
                }
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Generator generator;
    std::string result = generator.generate(*program);
    std::string expected = R"(<body><div style="width:100px;height:200px;"></div></body>)";
    ASSERT_EQ(result, expected);
}
