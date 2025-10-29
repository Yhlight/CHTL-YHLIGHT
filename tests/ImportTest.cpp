#include "gtest/gtest.h"
#include "TestHelpers.h"
#include <fstream>

TEST(ImportTest, SimpleImport) {
    std::ofstream outfile("import_target.chtl");
    outfile << "div { style { width: 100px; } }";
    outfile.close();

    std::string source = "[Import] @Chtl from \"import_target.chtl\";";
    std::string expected = "<div style=\"width:100px;\"></div>";
    EXPECT_EQ(removeWhitespace(compile(source, "main.chtl")), removeWhitespace(expected));
}

TEST(ImportTest, CircularImport) {
    std::ofstream outfile_a("a.chtl");
    outfile_a << "[Import] @Chtl from \"b.chtl\";";
    outfile_a.close();

    std::ofstream outfile_b("b.chtl");
    outfile_b << "[Import] @Chtl from \"a.chtl\";";
    outfile_b.close();

    std::string source = "[Import] @Chtl from \"a.chtl\";";
    expect_compile_error(source, "Circular import detected: a.chtl");
}
