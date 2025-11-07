#include "gtest/gtest.h"
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLGenerator/Generator.h"
#include "CHTLNode/ProgramNode.h"
#include <memory>
#include <stdexcept>

TEST(ValuelessStyleGroupTest, SimpleValuelessStyleGroup) {
    std::string source = R"(
        [Custom] @Style TextSet {
            color,
            font-size;
        }

        div {
            style {
                @Style TextSet {
                    color: red;
                    font-size: 16px;
                }
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Generator generator;
    std::string result = generator.generate(*program);
    std::string expected = R"(<div style="color:red;font-size:16px;"></div>)";
    ASSERT_EQ(result, expected);
}

TEST(ValuelessStyleGroupTest, ValuelessPropertyOutsideCustom) {
    std::string source = R"(
        [Template] @Style TextSet {
            color,
            font-size;
        }
        div{}
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    ASSERT_THROW(parser.parse(), std::runtime_error);
}
