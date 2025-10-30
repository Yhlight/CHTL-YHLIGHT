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

    std::remove("import_target.html");
}

TEST(NonChtlImportTest, ImportCssFile) {
    std::ofstream outfile("import_target.css");
    outfile << "body { color: red; }";
    outfile.close();

    std::string source = "[Import] @Style from \"import_target.css\" as myCss;";
    std::string expected = ""; // An import with an alias should not produce direct output
    EXPECT_EQ(removeWhitespace(compile(source, "main.chtl")), removeWhitespace(expected));

    std::remove("import_target.css");
}

TEST(NonChtlImportTest, ImportJsFile) {
    std::ofstream outfile("import_target.js");
    outfile << "console.log('Hello from JS');";
    outfile.close();

    std::string source = "[Import] @JavaScript from \"import_target.js\" as myJs;";
    std::string expected = ""; // An import with an alias should not produce direct output
    EXPECT_EQ(removeWhitespace(compile(source, "main.chtl")), removeWhitespace(expected));

    std::remove("import_target.js");
}

TEST(NonChtlImportTest, ImportAndUseHtml) {
    std::ofstream outfile("import_target.html");
    outfile << "<div>Imported</div>";
    outfile.close();

    std::string source = R"([Import] @Html from "import_target.html" as myHtml;
                           body { [Origin] @Html myHtml; })";
    std::string expected = "<body><div>Imported</div></body>";
    EXPECT_EQ(removeWhitespace(compile(source, "main.chtl")), removeWhitespace(expected));

    std::remove("import_target.html");
}

TEST(NonChtlImportTest, ImportAndUseCss) {
    std::ofstream outfile("import_target.css");
    outfile << "a { text-decoration: none; }";
    outfile.close();

    std::string source = R"([Import] @Style from "import_target.css" as myCss;
                           style { [Origin] @Style myCss; })";
    std::string expected = "<style>a { text-decoration: none; }</style>";
    EXPECT_EQ(removeWhitespace(compile(source, "main.chtl")), removeWhitespace(expected));

    std::remove("import_target.css");
}

TEST(NonChtlImportTest, ImportAndUseJs) {
    std::ofstream outfile("import_target.js");
    outfile << "alert('imported');";
    outfile.close();

    std::string source = R"([Import] @JavaScript from "import_target.js" as myJs;
                           script { [Origin] @JavaScript myJs; })";
    std::string expected = "<script>alert('imported');</script>";
    EXPECT_EQ(removeWhitespace(compile(source, "main.chtl")), removeWhitespace(expected));

    std::remove("import_target.js");
}
