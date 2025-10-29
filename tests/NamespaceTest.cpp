#include "gtest/gtest.h"
#include "TestHelpers.h"

TEST(NamespaceTest, SimpleNamespace) {
    std::string source = R"(
        [Namespace] my_ns {
            div {
                id: "my_div";
            }
        }
    )";
    std::string expected = "<div id=\"my_div\"></div>";
    EXPECT_EQ(removeWhitespace(compile(source)), removeWhitespace(expected));
}

TEST(NamespaceTest, NamespaceReferenceFail) {
    std::string source = R"(
        [Namespace] my_ns {
            div {
                id: "my_div";
            }
        }
        p {
            style {
                width: #my_div.width;
            }
        }
    )";
    expect_compile_error(source, "Unknown selector in property access: #my_div");
}
