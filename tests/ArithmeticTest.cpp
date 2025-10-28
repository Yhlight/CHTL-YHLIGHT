#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include <string>
#include <algorithm>

using namespace CHTL;

// Helper function to remove whitespace for comparison
static std::string removeWhitespace(std::string str) {
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
    return str;
}

TEST(ArithmeticTest, SimpleAddition) {
    std::string source = "div { style { width: 100 + 50; } }";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Generator generator(*program);
    std::string result = generator.generate();
    std::string expected = R"(<div style="width: 150;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ArithmeticTest, AdditionWithUnits) {
    std::string source = "div { style { width: 100px + 50px; } }";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Generator generator(*program);
    std::string result = generator.generate();
    std::string expected = R"(<div style="width: 150px;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ArithmeticTest, OperatorPrecedence) {
    std::string source = "div { style { width: 10 + 20 * 2; } }";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Generator generator(*program);
    std::string result = generator.generate();
    std::string expected = R"(<div style="width: 50;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ArithmeticTest, PowerOperator) {
    std::string source = "div { style { width: 2 ** 3; } }";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Generator generator(*program);
    std::string result = generator.generate();
    std::string expected = R"(<div style="width: 8;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}
