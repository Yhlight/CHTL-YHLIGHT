#include <gtest/gtest.h>
#include "TestHelpers.h"

TEST(ConditionalTest, SimpleTernaryTrue) {
    std::string source = R"(
        div {
            style {
                width: 1 > 0 ? 100px : 50px;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="width: 100px;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ConditionalTest, SimpleTernaryFalse) {
    std::string source = R"(
        div {
            style {
                background-color: 10 > 20 ? "red" : "blue";
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="background-color: "blue";"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ConditionalTest, LogicalAnd) {
    std::string source = R"(
        div {
            style {
                height: 10 > 5 && 3 < 4 ? 200px : 100px;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="height: 200px;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ConditionalTest, LogicalOr) {
    std::string source = R"(
        div {
            style {
                border: 1 > 5 || 3 < 4 ? "1px solid black" : "none";
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="border: "1pxsolidblack";"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ConditionalTest, StringComparison) {
    std::string source = R"(
        div {
            style {
                color: "hello" == "hello" ? green : "red";
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="color: green;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ConditionalTest, WithPropertyReference) {
    std::string source = R"(
        p {
            id: "ref";
            style {
                font-size: 16px;
            }
        }
        div {
            style {
                padding: #ref.font-size > 10px ? 20px : 5px;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<p id="ref" style="font-size: 16px;"></p><div style="padding: 20px;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}
