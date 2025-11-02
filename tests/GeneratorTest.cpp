#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLAnalyser/Analyser.h"
#include "CHTL/CHTLGenerator/Generator.h"

std::string compile(const std::string& source) {
    CHTL::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    auto ast = parser.parse();
    CHTL::Analyser analyser(*ast);
    analyser.analyse();
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

TEST(GeneratorTest, GeneratesElementWithInlineStyle) {
    std::string source = "div { style { color: red; font-size: 16px; } }";
    std::string expected = "<div style=\"color: red;font-size: 16px;\">\n</div>\n";
    EXPECT_EQ(compile(source), expected);
}

TEST(GeneratorTest, GeneratesElementWithStyleTemplate) {
    std::string source =
        "[Template] @Style MyTheme {"
        "    color: blue;"
        "}"
        "div {"
        "    style {"
        "        @Style MyTheme;"
        "    }"
        "}";
    std::string expected = "<div style=\"color: blue;\">\n</div>\n";
    EXPECT_EQ(compile(source), expected);
}
