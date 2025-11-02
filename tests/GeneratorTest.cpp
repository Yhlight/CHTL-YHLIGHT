#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"

std::string compile(const std::string& source) {
    CHTL::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    auto ast = parser.parse();
    CHTL::Generator generator(*ast);
    return generator.generate();
}

TEST(GeneratorTest, GeneratesSimpleElement) {
    std::string source = "div {}";
    std::string expected = "<div>\n</div>\n";
    EXPECT_EQ(compile(source), expected);
}

TEST(GeneratorTest, GeneratesNestedElements) {
    std::string source = "div { span {} }";
    std::string expected = "<div>\n  <span>\n  </span>\n</div>\n";
    EXPECT_EQ(compile(source), expected);
}

TEST(GeneratorTest, GeneratesElementWithText) {
    std::string source = "div { text { hello } }";
    std::string expected = "<div>\n   hello \n</div>\n";
    EXPECT_EQ(compile(source), expected);
}

TEST(GeneratorTest, GeneratesElementWithAttributes) {
    std::string source = "div { id: \"main\"; }";
    std::string expected = "<div id=\"main\">\n</div>\n";
    EXPECT_EQ(compile(source), expected);
}

TEST(GeneratorTest, GeneratesOriginBlock) {
    std::string source = "[Origin] @Html { <div>raw</div> }";
    std::string expected = " <div>raw</div> ";
    EXPECT_EQ(compile(source), expected);
}
