#include "gtest/gtest.h"
#include "TestHelpers.h"

TEST(OriginTest, BasicOrigin) {
    std::string chtl = R"(
[Origin] @Html
{    <h1>This is a raw HTML block</h1>
    <p>It should be rendered as is.</p>
}
)";
    std::string expected = R"(    <h1>This is a raw HTML block</h1>
    <p>It should be rendered as is.</p>

)";
    std::string generated = compile(chtl);
	ASSERT_EQ(generated.substr(generated.find("<h1>")), expected.substr(expected.find("<h1>")));
}
