#include "gtest/gtest.h"
#include "TestHelpers.h"
#include <fstream>

TEST(NonChtlImportTest, ImportHtmlFile) {
    std::ofstream outfile("import_target.html");
    outfile << "<h1>This is HTML</h1>";
    outfile.close();

    std::string source = "[Import] @Html from \"import_target.html\" as myHtml;";
    std::string expected = ""; // An import with an alias should not produce direct output
    EXPECT_EQ(removeWhitespace(compile(source, "main.chtl")), removeWhitespace(expected));
}

TEST(NonChtlImportTest, ImportCssFile) {
    std::ofstream outfile("import_target.css");
    outfile << "body { color: red; }";
    outfile.close();

    std::string source = "[Import] @Style from \"import_target.css\" as myCss;";
    std::string expected = ""; // An import with an alias should not produce direct output
    EXPECT_EQ(removeWhitespace(compile(source, "main.chtl")), removeWhitespace(expected));
}

TEST(NonChtlImportTest, ImportJsFile) {
    std::ofstream outfile("import_target.js");
    outfile << "console.log('Hello from JS');";
    outfile.close();

    std::string source = "[Import] @JavaScript from \"import_target.js\" as myJs;";
    std::string expected = ""; // An import with an alias should not produce direct output
    EXPECT_EQ(removeWhitespace(compile(source, "main.chtl")), removeWhitespace(expected));
}
