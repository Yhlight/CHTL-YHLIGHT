#include <gtest/gtest.h>
#include "TestHelpers.h"

TEST(CustomTest, StylePlaceholder) {
    std::string source = R"(
        [Custom] @Style TextSet {
            color,
            font-size;
        }

        div {
            style {
                @Style TextSet {
                    color: "red";
                    font-size: 16px;
                }
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="color: "red"; font-size: 16px;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(CustomTest, MissingValueError) {
    std::string source = R"(
        [Custom] @Style TextSet {
            color,
            font-size;
        }

        div {
            style {
                @Style TextSet {
                    color: "red";
                }
            }
        }
    )";
    expect_compile_error(source, "Missing value for placeholder 'font-size' in custom style 'TextSet'");
}

TEST(CustomTest, InheritanceWithPlaceholders) {
    std::string source = R"(
        [Template] @Style Base {
            padding: 10px;
        }

        [Custom] @Style Derived {
            inherit @Style Base;
            margin;
        }

        div {
            style {
                @Style Derived {
                    margin: 5px;
                }
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="padding: 10px; margin: 5px;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(CustomTest, Deletion) {
    std::string source = R"(
        [Template] @Style Base {
            color: "red";
            font-size: 16px;
            padding: 10px;
        }

        [Custom] @Style Derived {
            inherit @Style Base;
            delete font-size, padding;
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
