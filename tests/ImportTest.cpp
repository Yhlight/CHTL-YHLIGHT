#include "gtest/gtest.h"
#include "TestHelpers.h"
#include <fstream>
#include <cstdio>
#include <filesystem>

TEST(ImportTest, SimpleImport) {
    std::ofstream outfile("import_target.chtl");
    outfile << "div { style { width: 100px; } }";
    outfile.close();

    std::string source = "[Import] @Chtl from \"import_target.chtl\";";
    std::string expected = "<div style=\"width: 100px;\"></div>";
    EXPECT_EQ(removeWhitespace(compile(source, "main.chtl")), removeWhitespace(expected));

    std::remove("import_target.chtl");
}

TEST(ImportTest, CircularImport) {
    std::ofstream outfile_a("a.chtl");
    outfile_a << "[Import] @Chtl from \"b.chtl\";";
    outfile_a.close();

    std::ofstream outfile_b("b.chtl");
    outfile_b << "[Import] @Chtl from \"a.chtl\";";
    outfile_b.close();

    std::string source = "[Import] @Chtl from \"a.chtl\";";
    // The path in the error message is canonicalized, so we need to build it
    std::string expected_error = "Circular import detected: a.chtl";
    expect_compile_error(source, expected_error);

    std::remove("a.chtl");
    std::remove("b.chtl");
}

TEST(ImportTest, RawHtmlImport) {
    std::ofstream outfile("imported.html");
    outfile << "<h1>Imported</h1>";
    outfile.close();

    std::string source = "[Import] @Html from \"imported.html\" as myHtml;";
    std::string expected = "<h1>Imported</h1>";
    EXPECT_EQ(removeWhitespace(compile(source, "main.chtl")), removeWhitespace(expected));

    std::remove("imported.html");
}

TEST(ImportTest, RawCssImport) {
    std::ofstream outfile("imported.css");
    outfile << "body { color: red; }";
    outfile.close();

    std::string source = "[Import] @Style from \"imported.css\" as myCss;";
    std::string expected = "body { color: red; }";
    EXPECT_EQ(removeWhitespace(compile(source, "main.chtl")), removeWhitespace(expected));

    std::remove("imported.css");
}

TEST(ImportTest, RawJsImport) {
    std::ofstream outfile("imported.js");
    outfile << "console.log('imported');";
    outfile.close();

    std::string source = "[Import] @JavaScript from \"imported.js\" as myJs;";
    std::string expected = "console.log('imported');";
    EXPECT_EQ(removeWhitespace(compile(source, "main.chtl")), removeWhitespace(expected));

    std::remove("imported.js");
}

TEST(ImportTest, RawImportWithoutAlias) {
    std::ofstream outfile("imported.html");
    outfile << "test";
    outfile.close();

    std::string source = "[Import] @Html from \"imported.html\";";
	std::string expected = "test";
    EXPECT_EQ(removeWhitespace(compile(source, "main.chtl")), removeWhitespace(expected));

    std::remove("imported.html");
}
