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

TEST(CustomTest, ElementSpecializationWithIndex) {
    std::string source = R"(
        [Custom] @Element MyComponent {
            div {}
            div {}
            span {}
        }

        @Element MyComponent {
            div[1] {
                style {
                    color: "blue";
                }
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div></div><div style="color: "blue";"></div><span></span>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(CustomTest, ElementDeletionWithIndex) {
    std::string source = R"(
        [Custom] @Element MyComponent {
            div {}
            p {}
            div {}
            span {}
        }

        @Element MyComponent {
            delete div[1];
            delete p[0];
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div></div><span></span>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(CustomTest, ElementInsertion) {
    std::string source = R"(
        [Custom] @Element MyComponent {
            div {}
            span {}
        }

        @Element MyComponent {
            insert after div[0] {
                p {}
            }
            insert before span[0] {
                hr {}
            }
            insert at top {
                h1 {}
            }
            insert at bottom {
                h2 {}
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<h1></h1><div></div><p></p><hr /><span></span><h2></h2>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}
