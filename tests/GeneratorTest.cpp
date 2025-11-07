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

TEST(GeneratorTest, GeneratesValuelessStyleGroup) {
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

    ASSERT_EQ(result, "<div style=\"color:red;font-size:16px;\"></div>");
}

TEST(GeneratorTest, HandlesCustomTemplatePropertyDeletion) {
    std::string source = R"(
        [Template] @Style Parent {
            color: "blue";
            font-size: 16px;
        }
        [Custom] @Style Child {
            @Style Parent;
            delete color;
        }
        div {
            style {
                @Style Child;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<div style=\"font-size:16px;\"></div>");
}

TEST(GeneratorTest, HandlesCustomTemplateInheritanceDeletion) {
    std::string source = R"(
        [Template] @Style Grandparent {
            font-weight: bold;
        }
        [Template] @Style Parent {
            @Style Grandparent;
            color: "blue";
        }
        [Custom] @Style Child {
            @Style Parent;
            delete @Style Parent;
        }
        div {
            style {
                @Style Child;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<div></div>");
}

TEST(GeneratorTest, HandlesMixedCustomTemplate) {
    std::string source = R"(
        [Template] @Style StyleA {
            color: "blue";
        }
        [Template] @Style StyleB {
            font-size: 16px;
        }
        [Custom] @Style StyleC {
            @Style StyleA;
            @Style StyleB;
            delete @Style StyleA;
            delete font-size;
        }
        div {
            style {
                @Style StyleC;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<div></div>");
}

TEST(GeneratorTest, GeneratesSingleLevelInheritance) {
    std::string source = R"(
        [Template] @Style Parent {
            color: "blue";
        }
        [Template] @Style Child {
            @Style Parent;
            font-size: 16px;
        }
        div {
            style {
                @Style Child;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<div style=\"color:blue;font-size:16px;\"></div>");
}

TEST(GeneratorTest, GeneratesMultiLevelInheritance) {
    std::string source = R"(
        [Template] @Style Grandparent {
            color: "green";
        }
        [Template] @Style Parent {
            @Style Grandparent;
            font-size: 16px;
        }
        [Template] @Style Child {
            @Style Parent;
            font-weight: bold;
        }
        div {
            style {
                @Style Child;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<div style=\"color:green;font-size:16px;font-weight:bold;\"></div>");
}

TEST(GeneratorTest, HandlesPropertyOverrides) {
    std::string source = R"(
        [Template] @Style Parent {
            color: "blue";
            font-size: 16px;
        }
        [Template] @Style Child {
            @Style Parent;
            color: "red";
        }
        div {
            style {
                @Style Child;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<div style=\"color:red;font-size:16px;\"></div>");
}

TEST(GeneratorTest, DetectsCircularDependencies) {
    std::string source = R"(
        [Template] @Style A {
            @Style B;
        }
        [Template] @Style B {
            @Style A;
        }
        div {
            style {
                @Style A;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    ASSERT_THROW(generator.generate(*program), std::runtime_error);
}

TEST(GeneratorTest, GeneratesVariableTemplate) {
    std::string source = R"(
        [Template] @Var Theme {
            textColor: "black";
        }
        div {
            style {
                color: Theme(textColor);
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
