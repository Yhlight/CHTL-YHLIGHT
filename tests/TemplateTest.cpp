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

TEST(TemplateTest, StyleImplicitInheritance) {
    std::string source = R"(
        [Template] @Style Base {
            color: "red";
            font-size: 16px;
        }

        [Template] @Style Derived {
            @Style Base;
            font-weight: bold;
        }

        div {
            style {
                @Style Derived;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="color: "red"; font-size: 16px; font-weight: bold;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(TemplateTest, StyleExplicitInheritance) {
    std::string source = R"(
        [Template] @Style Base {
            color: "red";
        }

        [Template] @Style Derived {
            inherit @Style Base;
            font-weight: bold;
        }

        div {
            style {
                @Style Derived;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="color: "red"; font-weight: bold;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(TemplateTest, PropertyOverride) {
    std::string source = R"(
        [Template] @Style Base {
            color: "red";
            font-size: 16px;
        }

        [Template] @Style Derived {
            @Style Base;
            color: "blue";
        }

        div {
            style {
                @Style Derived;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="font-size: 16px; color: "blue";"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(TemplateTest, ElementInheritance) {
    std::string source = R"(
        [Template] @Element Base {
            p { text { "Base" } }
        }

        [Template] @Element Derived {
            @Element Base;
            div { text { "Derived" } }
        }

        body {
            @Element Derived;
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<body><p>Base</p><div>Derived</div></body>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(TemplateTest, MultiLevelInheritance) {
    std::string source = R"(
        [Template] @Style L1 {
            padding: 10px;
        }

        [Template] @Style L2 {
            @Style L1;
            margin: 5px;
        }

        [Template] @Style L3 {
            @Style L2;
            border: 1px solid black;
        }

        div {
            style {
                @Style L3;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="padding: 10px; margin: 5px; border: 1px solid black;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(TemplateTest, CircularInheritanceError) {
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
    expect_compile_error(source, "Circular template inheritance detected: B");
}
