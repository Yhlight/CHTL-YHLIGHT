#include <gtest/gtest.h>
#include "TestHelpers.h"

TEST(TemplateTest, StyleTemplate) {
    std::string source = R"(
        [Template] @Style DefaultText {
            color: "black";
            font-size: 16px;
        }

        div {
            style {
                @Style DefaultText;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="color: "black"; font-size: 16px;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(TemplateTest, ElementTemplate) {
    std::string source = R"(
        [Template] @Element Box {
            div {
                class: "box";
            }
        }

        body {
            @Element Box;
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<body><div class="box"></div></body>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(TemplateTest, VarTemplate) {
    std::string source = R"(
        [Template] @Var Theme {
            primary-color: "blue";
        }

        div {
            style {
                color: Theme.primary-color;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="color: "blue";"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}
