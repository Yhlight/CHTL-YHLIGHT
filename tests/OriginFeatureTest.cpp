#include "gtest/gtest.h"
#include "TestHelpers.h"

TEST(OriginFeatureTest, NamedOriginDefinitionAndUsage) {
    std::string chtl = R"(
[Origin] @Html namedBlock
{<p>This is a named raw block.</p>}

html {
    body {
        [Origin] @Html namedBlock;
    }
}
)";
    // The generator outputs raw content exactly as is, without adding indentation.
    // The newline after the body content is also expected from the generator's current behavior.
    std::string expected = R"(<html>
<body>
<p>This is a named raw block.</p></body>
</html>
)";
    std::string generated = compile(chtl);

    // Normalize newlines and remove trailing newline from generator if present
    expected.erase(std::remove(expected.begin(), expected.end(), '\r'), expected.end());
    generated.erase(std::remove(generated.begin(), generated.end(), '\r'), generated.end());
    if (!expected.empty() && expected.back() == '\n') expected.pop_back();
    if (!generated.empty() && generated.back() == '\n') generated.pop_back();


    ASSERT_EQ(generated, expected);
}

TEST(OriginFeatureTest, NamedOriginUsageInsideElement) {
    std::string chtl = R"(
[Origin] @Html anotherBlock {<span>Another raw block.</span>}

div {
    [Origin] @Html anotherBlock;
}
)";
    // The generator does not add indentation to the raw content.
    std::string expected = R"(<div>
<span>Another raw block.</span></div>
)";
    std::string generated = compile(chtl);

    expected.erase(std::remove(expected.begin(), expected.end(), '\r'), expected.end());
    generated.erase(std::remove(generated.begin(), generated.end(), '\r'), generated.end());
    if (!expected.empty() && expected.back() == '\n') expected.pop_back();
    if (!generated.empty() && generated.back() == '\n') generated.pop_back();

    ASSERT_EQ(generated, expected);
}

TEST(OriginFeatureTest, UnknownNamedOriginUsage) {
    std::string chtl = R"(
div {
    [Origin] @Html nonExistentBlock;
}
)";
    expect_compile_error(chtl, "Unknown origin block alias: nonExistentBlock");
}
