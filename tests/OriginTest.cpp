#include "gtest/gtest.h"
#include "TestHelpers.h"

TEST(OriginTest, BasicOrigin) {
    std::string chtl = R"(
[Origin] @Html
{<h1>This is a raw HTML block</h1>}
)";
    std::string expected = R"(<h1>This is a raw HTML block</h1>)";
    std::string generated = compile(chtl);
	ASSERT_EQ(generated, expected);
}

TEST(OriginTest, StyleOrigin) {
    std::string chtl = R"(
[Origin] @Style
{.raw-css { color: blue; }}
)";
    std::string expected = R"(.raw-css { color: blue; })";
    std::string generated = compile(chtl);
    ASSERT_EQ(generated, expected);
}

TEST(OriginTest, JavaScriptOrigin) {
    std::string chtl = R"(
[Origin] @JavaScript
{console.log("raw js");}
)";
    std::string expected = R"(console.log("raw js");)";
    std::string generated = compile(chtl);
    ASSERT_EQ(generated, expected);
}

TEST(OriginTest, NestedOrigin) {
    std::string chtl = R"(
html {
    body {
        div {
            [Origin] @Html
            {<span>Raw Content</span>}
        }
    }
}
)";
    std::string expected = R"(
<html>
<body>
<div>
<span>Raw Content</span></div>
</body>
</html>
)";
    // Normalize newlines for cross-platform compatibility
    expected.erase(std::remove(expected.begin(), expected.end(), '\r'), expected.end());
    std::string generated = compile(chtl);
    generated.erase(std::remove(generated.begin(), generated.end(), '\r'), generated.end());

    ASSERT_EQ(generated, expected);
}
