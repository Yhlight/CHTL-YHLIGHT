#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include <string>
#include <algorithm>

using namespace CHTL;

// Helper function from GeneratorTest.cpp
inline std::string removeWhitespace(std::string str) {
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
    return str;
}


TEST(StyleTest, ParsesAndGeneratesInlineStyles) {
    std::string source = R"(
        div {
            style {
                color: red;
                font-size: 16px;
            }
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();

    // Test Parser
    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->children.size(), 1);
    auto* div = dynamic_cast<ElementNode*>(program->children[0].get());
    ASSERT_NE(div, nullptr);
    ASSERT_NE(div->style, nullptr);
    ASSERT_EQ(div->style->properties.size(), 2);
    EXPECT_EQ(div->style->properties[0].key, "color");
    EXPECT_EQ(div->style->properties[0].value, "red");
    EXPECT_EQ(div->style->properties[1].key, "font-size");
    EXPECT_EQ(div->style->properties[1].value, "16px");

    // Test Generator
    Generator generator(*program);
    std::string result = generator.generate();
    std::string expected = R"(<div style="color: red; font-size: 16px;"></div>)";

    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(StyleTest, HandlesEmptyStyleBlock) {
    std::string source = "div { style {} }";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();

    // Test Parser
    ASSERT_NE(program, nullptr);
    auto* div = dynamic_cast<ElementNode*>(program->children[0].get());
    ASSERT_NE(div, nullptr);
    ASSERT_NE(div->style, nullptr);
    EXPECT_TRUE(div->style->properties.empty());

    // Test Generator
    Generator generator(*program);
    std::string result = generator.generate();
    std::string expected = "<div></div>";

    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(StyleTest, CombinesAttributesAndStyles) {
    std::string source = R"(
        a {
            href: "https://example.com";
            style {
                text-decoration: none;
            }
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Generator generator(*program);
    std::string result = generator.generate();
    std::string expected = R"(<a href="https://example.com" style="text-decoration: none;"></a>)";

    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(StyleTest, GeneratesAutoClass) {
    std::string source = "div { style { .box {} } }";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Generator generator(*program);
    std::string result = generator.generate();
    std::string expected = R"(<div class="box"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(StyleTest, GeneratesAutoId) {
    std::string source = "div { style { #main {} } }";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Generator generator(*program);
    std::string result = generator.generate();
    std::string expected = R"(<div id="main"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(StyleTest, DoesNotOverrideExistingClass) {
    std::string source = R"(div { class: "manual"; style { .auto {} } })";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Generator generator(*program);
    std::string result = generator.generate();
    std::string expected = R"(<div class="manual"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(StyleTest, UsesFirstAutoSelector) {
    std::string source = "div { style { .first {} #second {} } }";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Generator generator(*program);
    std::string result = generator.generate();
    std::string expected = R"(<div class="first" id="second"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(StyleTest, GeneratesGlobalStyleForPseudoClass) {
    std::string source = R"(
        html {
            body {
                a {
                    class: "link";
                    style {
                        &:hover {
                            color: blue;
                        }
                    }
                }
            }
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Generator generator(*program);
    std::string result = generator.generate();

    std::string expected_html = R"(
        <html>
            <head>
                <style>
                    .link:hover {
                        color: blue;
                    }
                </style>
            </head>
            <body>
                <a class="link"></a>
            </body>
        </html>
    )";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected_html));
}

TEST(StyleTest, GeneratesGlobalStyleForPseudoElement) {
    std::string source = R"(
        html {
            div {
                id: "main";
                style {
                    &::before {
                        content: "Hello";
                    }
                }
            }
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Generator generator(*program);
    std::string result = generator.generate();

    std::string expected_html = R"(
        <html>
            <head>
                <style>
                    #main::before {
                        content: "Hello";
                    }
                </style>
            </head>
            <div id="main"></div>
        </html>
    )";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected_html));
}
