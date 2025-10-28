#include "gtest/gtest.h"
#include "CHTL/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include <string>

// Test fixture for Template Inheritance tests
class TemplateInheritanceTest : public ::testing::Test {
protected:
    std::string parseAndGenerate(const std::string& input) {
        CHTL::Lexer lexer(input);
        CHTL::Parser parser(lexer);
        auto program = parser.parse();
        CHTL::Generator generator(program, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates());
        return generator.generate();
    }
};

TEST_F(TemplateInheritanceTest, StyleTemplateInheritance) {
    std::string chtlCode = R"(
        [Template] @Style BaseStyle {
            color: "red";
            font-size: 16px;
        }

        [Template] @Style ChildStyle {
            inherit @Style BaseStyle;
            font-size: 20px;
        }

        div {
            style {
                @Style ChildStyle;
            }
        }
    )";
    std::string expectedHtml = R"(<div style="color:red;font-size:20px;"></div>)";
    ASSERT_EQ(parseAndGenerate(chtlCode), expectedHtml);
}

TEST_F(TemplateInheritanceTest, StyleTemplateExplicitInheritance) {
    std::string chtlCode = R"(
        [Template] @Style BaseStyle {
            color: "red";
        }

        [Template] @Style ChildStyle {
            inherit @Style BaseStyle;
        }

        div {
            style {
                @Style ChildStyle;
            }
        }
    )";
    std::string expectedHtml = R"(<div style="color:red;"></div>)";
    ASSERT_EQ(parseAndGenerate(chtlCode), expectedHtml);
}


TEST_F(TemplateInheritanceTest, ElementTemplateInheritance) {
    std::string chtlCode = R"(
        [Template] @Element BaseElement {
            p { text: "Base"; }
        }

        [Template] @Element ChildElement {
            inherit @Element BaseElement;
            div { text: "Child"; }
        }

        @Element ChildElement;
    )";
    std::string expectedHtml = "<p>Base</p><div>Child</div>";
    ASSERT_EQ(parseAndGenerate(chtlCode), expectedHtml);
}

TEST_F(TemplateInheritanceTest, VarTemplateInheritance) {
    std::string chtlCode = R"(
        [Template] @Var BaseVars {
            primaryColor: "blue";
            secondaryColor: "gray";
        }

        [Template] @Var ChildVars {
            inherit @Var BaseVars;
            primaryColor: "red";
        }

        div {
            style {
                color: ChildVars(primaryColor);
                background-color: ChildVars(secondaryColor);
            }
        }
    )";
    std::string expectedHtml = R"(<div style="background-color:gray;color:red;"></div>)";
    ASSERT_EQ(parseAndGenerate(chtlCode), expectedHtml);
}
