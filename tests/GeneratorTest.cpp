#include <gtest/gtest.h>
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLGenerator/Generator.h"

TEST(GeneratorTest, GeneratesElementWithText) {
    std::string source = "p { text { \"hello\" } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<p>hello</p>");
}

TEST(GeneratorTest, GeneratesElementWithAttributes) {
    std::string source = "div { id: \"main\" }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<div id=\"main\"></div>");
}

TEST(GeneratorTest, GeneratesInlineStyle) {
    std::string source = "div { style { color: red; } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<div style=\"color:red;\"></div>");
}

TEST(GeneratorTest, GeneratesGlobalStylesheet) {
    std::string source = "div { style { .box { color: blue; } } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<style>.box{color:blue;}</style><div class=\"box\"></div>");
}

TEST(GeneratorTest, HandlesAutomaticClassAndId) {
    std::string source = "div { class:\"existing\"; style { .box { color: blue; } #main { background: white; } } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<style>.box{color:blue;}#main{background:white;}</style><div class=\"existing box\" id=\"main\"></div>");
}

TEST(GeneratorTest, HandlesContextDeduction) {
    std::string source = "div { style { .box { color: blue; } &:hover { color: red; } } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<style>.box{color:blue;}.box:hover{color:red;}</style><div class=\"box\"></div>");
}

TEST(GeneratorTest, HandlesContextDeductionWithExistingClass) {
    std::string source = "div { class:\"existing\"; style { .box { color: blue; } &:hover { color: red; } } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<style>.box{color:blue;}.box:hover{color:red;}</style><div class=\"existing box\"></div>");
}

TEST(GeneratorTest, GeneratesScriptBlock) {
    std::string source = R"(div { script { "console.log('hello');" } })";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<div><script>console.log('hello');</script></div>");
}

TEST(GeneratorTest, GeneratesOriginHtml) {
    std::string source = R"([Origin] @Html { "<div><p>raw html</p></div>" })";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<div><p>raw html</p></div>");
}

TEST(GeneratorTest, GeneratesOriginCss) {
    std::string source = R"([Origin] @Style { ".raw-css { color: purple; }" })";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<style>.raw-css { color: purple; }</style>");
}

TEST(GeneratorTest, GeneratesOriginJavaScript) {
    std::string source = R"([Origin] @JavaScript { "alert('raw js');" })";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "alert('raw js');");
}

TEST(GeneratorTest, GeneratesElementTemplate) {
    std::string source = R"(
        [Template] @Element Box {
            div {
                text { "Hello" }
            }
        }
        @Element Box;
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<div>Hello</div>");
}

TEST(GeneratorTest, GeneratesStyleTemplate) {
    std::string source = R"(
        [Template] @Style DefaultText {
            color: "black";
        }
        div {
            style {
                @Style DefaultText;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<div style=\"color:black;\"></div>");
}
