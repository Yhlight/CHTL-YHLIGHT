#include "gtest/gtest.h"
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLGenerator/Generator.h"
#include "CHTLNode/ProgramNode.h"
#include <memory>

TEST(ArithmeticExpressionTest, SimpleAddition) {
    std::string source = R"(
        div {
            style {
                width: 10px + 20px;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Generator generator;
    std::string result = generator.generate(*program);
    std::string expected = R"(<div style="width:calc(10px + 20px);"></div>)";
    ASSERT_EQ(result, expected);
}

TEST(ArithmeticExpressionTest, OperatorPrecedence) {
    std::string source = R"(
        div {
            style {
                width: 10px + 20px * 2;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Generator generator;
    std::string result = generator.generate(*program);
    std::string expected = R"(<div style="width:calc(10px + calc(20px * 2));"></div>)";
    ASSERT_EQ(result, expected);
}

TEST(ArithmeticExpressionTest, Parentheses) {
    std::string source = R"(
        div {
            style {
                width: (10px + 20px) * 2;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Generator generator;
    std::string result = generator.generate(*program);
    std::string expected = R"(<div style="width:calc(calc(10px + 20px) * 2);"></div>)";
    ASSERT_EQ(result, expected);
}

TEST(ArithmeticExpressionTest, AllOperators) {
    std::string source = R"(
        div {
            style {
                width: 10px + 20px * 2 - 5px / 2 % 3 ** 2;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Generator generator;
    std::string result = generator.generate(*program);
    std::string expected = R"(<div style="width:calc(calc(10px + calc(20px * 2)) - calc(calc(5px / 2) % calc(3 ** 2)));"></div>)";
    ASSERT_EQ(result, expected);
}
