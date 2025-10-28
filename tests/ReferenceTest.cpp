#include <gtest/gtest.h>
#include "TestHelpers.h"

TEST(ReferenceTest, SimpleReference) {
    std::string source = R"(
        div {
            class: "box";
            style {
                width: 100px;
            }
        }
        p {
            style {
                width: .box.width;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div class="box" style="width: 100px;"></div><p style="width: 100px;"></p>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ReferenceTest, ReferenceWithArithmetic) {
    std::string source = R"(
        div {
            class: "box";
            style {
                width: 100px;
            }
        }
        p {
            style {
                width: .box.width + 50px;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div class="box" style="width: 100px;"></div><p style="width: 150px;"></p>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ReferenceTest, ReferenceById) {
    std::string source = R"(
        div {
            id: "main";
            style {
                height: 200px;
            }
        }
        p {
            style {
                height: #main.height - 50px;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div id="main" style="height: 200px;"></div><p style="height: 150px;"></p>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ReferenceTest, ReferenceByTag) {
    std::string source = R"(
        div {
            style {
                padding: 10px;
            }
        }
        p {
            style {
                margin: div.padding;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="padding: 10px;"></div><p style="margin: 10px;"></p>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ReferenceTest, ReferenceCalculatedValue) {
    std::string source = R"(
        div {
            class: "container";
            style {
                width: 100px * 2;
            }
        }
        p {
            style {
                width: .container.width / 4;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div class="container" style="width: 200px;"></div><p style="width: 50px;"></p>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}
