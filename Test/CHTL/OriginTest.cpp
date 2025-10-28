#include "gtest/gtest.h"
#include "CHTL/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include <string>

class OriginTest : public ::testing::Test {
protected:
    std::string parseAndGenerate(const std::string& input) {
        CHTL::Lexer lexer(input);
        CHTL::Parser parser(lexer);
        auto program = parser.parse();
        CHTL::Generator generator(program, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates(), parser.getOriginBlocks());
        return generator.generate();
    }
};

TEST_F(OriginTest, UnnamedOriginHtml) {
    std::string chtlCode = R"(
        [Origin] @Html {<h1>This is raw HTML</h1>}
    )";
    std::string expectedHtml = "<h1>This is raw HTML</h1>";
    ASSERT_EQ(parseAndGenerate(chtlCode), expectedHtml);
}

TEST_F(OriginTest, NamedOriginStyle) {
    std::string chtlCode = R"(
        [Origin] @Style myStyles {body { margin: 0; }}
    )";
    std::string expectedHtml = "";
    ASSERT_EQ(parseAndGenerate(chtlCode), expectedHtml);
}

TEST_F(OriginTest, OriginJavaScript) {
    std::string chtlCode = R"(
        [Origin] @JavaScript {console.log("Hello, from raw JS!");}
    )";
    std::string expectedHtml = "console.log(\"Hello, from raw JS!\");";
    ASSERT_EQ(parseAndGenerate(chtlCode), expectedHtml);
}

TEST_F(OriginTest, NamedOriginUsage) {
    std::string chtlCode = R"(
        [Origin] @Html myBlock {<div>Hello</div>}
        [Origin] @Html myBlock;
    )";
    std::string expectedHtml = "<div>Hello</div>";
    ASSERT_EQ(parseAndGenerate(chtlCode), expectedHtml);
}
