#include <gtest/gtest.h>
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"

void runGeneratorTest(const std::string& source, const std::string& expectedOutput) {
    CHTL::Parser parser(source);
    auto program = parser.parse();
    CHTL::Generator generator;
    std::string output = generator.generate(program.get());
    EXPECT_EQ(output, expectedOutput);
}

TEST(GeneratorTest, GenerateSingleElement) {
    std::string source = "div {}";
    std::string expected = "<div></div>";
    runGeneratorTest(source, expected);
}

TEST(GeneratorTest, GenerateNestedElements) {
    std::string source = "html { body { div {} } }";
    std::string expected = "<html><body><div></div></body></html>";
    runGeneratorTest(source, expected);
}

TEST(GeneratorTest, GenerateTextNode) {
    std::string source = "text { \"hello\" }";
    std::string expected = "hello";
    runGeneratorTest(source, expected);
}

TEST(GeneratorTest, GenerateElementWithText) {
    std::string source = "p { text { \"world\" } }";
    std::string expected = "<p>world</p>";
    runGeneratorTest(source, expected);
}

TEST(GeneratorTest, GenerateAttributes) {
    std::string source = "div { id: \"main\"; class = box; }";
    std::string expected = "<div class=\"box\" id=\"main\"></div>";
    runGeneratorTest(source, expected);
}

TEST(GeneratorTest, GenerateInlineStyle) {
    std::string source = "div { style { color: red; } }";
    std::string expected = "<div style=\" color: red; \"></div>";
    runGeneratorTest(source, expected);
}
