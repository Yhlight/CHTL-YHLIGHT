#include "gtest/gtest.h"
#include "TestHelpers.h"

TEST(OriginTest, BasicOrigin) {
    std::string chtl = R"(
html{
    body{
        [Origin] @Html
        {    <h1>This is a raw HTML block</h1>
    <p>It should be rendered as is.</p>}
    }
}
)";
    std::string expected = R"(<html>
<body>
    <h1>This is a raw HTML block</h1>
    <p>It should be rendered as is.</p></body>
</html>
)";
    std::string generated = compile(chtl);
	ASSERT_EQ(generated, expected);
}
