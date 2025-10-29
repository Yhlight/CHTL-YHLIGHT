#include "gtest/gtest.h"
#include "TestHelpers.h"

TEST(NamespaceTest, SimpleNamespace) {
    std::string source = R"(
        [Namespace] my_ns {
            [Template] @Element MyTemplate {
                div {
                    id: "my_div";
                }
            }
        }
        @Element MyTemplate from my_ns;
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

TEST(NamespaceTest, TemplateInNamespace) {
    std::string source = R"(
        [Namespace] my_ns {
            [Template] @Element MyTemplate {
                div {
                    id: "template_div";
                }
            }
        }
        @Element MyTemplate from my_ns;
    )";
    std::string expected = R"(<div id="template_div"></div>)";
    EXPECT_EQ(removeWhitespace(compile(source)), removeWhitespace(expected));
}

TEST(NamespaceTest, AutoNamespaceOnImport) {
    createFile("test_import.chtl", R"(
        [Template] @Element MyTemplate {
            p {
                id: "imported_p";
            }
        }
    )");

    std::string source = R"(
        [Import] @Chtl from "test_import.chtl" as my_import;
        @Element MyTemplate from my_import;
    )";
    std::string expected = R"(<p id="imported_p"></p>)";
    EXPECT_EQ(removeWhitespace(compile(source)), removeWhitespace(expected));
}

TEST(NamespaceTest, PseudoMerge) {
    std::string source = R"(
        [Namespace] ns1 {
            [Template] @Element T1 {
                div {}
            }
        }
        [Namespace] ns1 {
            [Template] @Element T2 {
                p {}
            }
        }
        @Element T1 from ns1;
        @Element T2 from ns1;
    )";
    std::string expected = "<div></div><p></p>";
    EXPECT_EQ(removeWhitespace(compile(source)), removeWhitespace(expected));
}
