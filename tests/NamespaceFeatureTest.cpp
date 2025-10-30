#include "gtest/gtest.h"
#include "TestHelpers.h"

TEST(NamespaceFeatureTest, BasicNamespace) {
    std::string source = R"(
        [Namespace] mySpace {
            [Template] @Element MyElement {
                div { text { "Hello from namespace" } }
            }
        }
        body {
            @Element MyElement from mySpace;
        }
    )";
    std::string expected = R"(
        <body>
            <div>Hello from namespace</div>
        </body>
    )";
    EXPECT_EQ(removeWhitespace(compile(source)), removeWhitespace(expected));
}

TEST(NamespaceFeatureTest, NestedNamespace) {
    std::string source = R"(
        [Namespace] outerSpace {
            [Namespace] innerSpace {
                [Template] @Element MyElement {
                    span { text { "Nested namespace" } }
                }
            }
        }
        body {
            @Element MyElement from outerSpace.innerSpace;
        }
    )";
    std::string expected = R"(
        <body>
            <span>Nested namespace</span>
        </body>
    )";
    EXPECT_EQ(removeWhitespace(compile(source)), removeWhitespace(expected));
}

TEST(NamespaceFeatureTest, UnqualifiedAccessFails) {
    std::string source = R"(
        [Namespace] mySpace {
            [Template] @Element MyElement {
                div {}
            }
        }
        body {
            @Element MyElement;
        }
    )";
    expect_compile_error(source, "Unknown template: MyElement");
}
