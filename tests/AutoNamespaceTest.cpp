#include "gtest/gtest.h"
#include "TestHelpers.h"
#include <fstream>

TEST(AutoNamespaceTest, ImportWithoutNamespace) {
    std::ofstream outfile("import_target.chtl");
    outfile << R"([Template] @Element MyElement { div { text: "Hello"; } })";
    outfile.close();

    std::string source = R"([Import] @Chtl from "import_target.chtl";
                           body { @Element MyElement from import_target; })";
    std::string expected = R"(<body><div>Hello</div></body>)";
    EXPECT_EQ(removeWhitespace(compile(source, "main.chtl")), removeWhitespace(expected));

    std::remove("import_target.chtl");
}
