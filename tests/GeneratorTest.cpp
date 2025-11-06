#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"

// Helper function to compile CHTL to HTML
std::string compileCHTL(const std::string& source) {
    Lexer lexer(source);
    Parser parser(lexer);
    auto program = parser.parse();
    if (!program) {
        return "Parsing failed";
    }
    Generator generator;
    return generator.generate(*program);
}

TEST(GeneratorTest, GenerateSingleElement) {
    std::string source = "html {}";
    std::string expected = "<html></html>";
    EXPECT_EQ(compileCHTL(source), expected);
}

TEST(GeneratorTest, GenerateElementWithText) {
    std::string source = "p { text { \"Hello CHTL\" } }";
    std::string expected = "<p>Hello CHTL</p>";
    EXPECT_EQ(compileCHTL(source), expected);
}

TEST(GeneratorTest, GenerateElementWithAttributes) {
    std::string source = "a { href: \"https://example.com\"; target: \"_blank\"; }";
    std::string expected = "<a href=\"https://example.com\" target=\"_blank\"></a>";
    EXPECT_EQ(compileCHTL(source), expected);
}

TEST(GeneratorTest, GenerateNestedElements) {
    std::string source = "div { id: \"parent\"; p { class: \"child\"; text { \"nested\" } } }";
    std::string expected = "<div id=\"parent\"><p class=\"child\">nested</p></div>";
    EXPECT_EQ(compileCHTL(source), expected);
}

TEST(GeneratorTest, GenerateMultipleTopLevelElements) {
    std::string source = "h1 {} p {}";
    std::string expected = "<h1></h1><p></p>";
    EXPECT_EQ(compileCHTL(source), expected);
}

TEST(GeneratorTest, GenerateComplexStructure) {
    std::string source = R"(
        body {
            div {
                id: "main-content";
                h1 {
                    class: "title";
                    text { "Welcome to CHTL" }
                }
                p {
                    text { "This is a paragraph." }
                }
            }
        }
    )";
    std::string expected = "<body><div id=\"main-content\"><h1 class=\"title\">Welcome to CHTL</h1><p>This is a paragraph.</p></div></body>";
    EXPECT_EQ(compileCHTL(source), expected);
}

TEST(StyleBlockTest, GenerateSingleStyleProperty) {
    std::string source = "div { style { color: red; } }";
    std::string expected = "<div style=\"color: red\"></div>";
    EXPECT_EQ(compileCHTL(source), expected);
}

TEST(StyleBlockTest, GenerateMultipleStyleProperties) {
    std::string source = "p { style { font-size: 16px; font-weight: bold; } }";
    std::string expected = "<p style=\"font-size: 16px;font-weight: bold\"></p>";
    EXPECT_EQ(compileCHTL(source), expected);
}

TEST(StyleBlockTest, GenerateStylesAndAttributes) {
    std::string source = "a { class: \"link\"; style { text-decoration: none; } }";
    std::string expected = "<a class=\"link\" style=\"text-decoration: none\"></a>";
    EXPECT_EQ(compileCHTL(source), expected);
}

TEST(StyleBlockTest, GenerateStylesAndNestedContent) {
    std::string source = "div { style { border: '1px solid black'; } text { \"content\" } }";
    std::string expected = "<div style=\"border: 1px solid black\">content</div>";
    EXPECT_EQ(compileCHTL(source), expected);
}
