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

#include <filesystem>

TEST(ImportTest, CircularImport) {
    std::ofstream outfile_a("a.chtl");
    outfile_a << "[Import] @Chtl from \"b.chtl\";";
    outfile_a.close();

    std::ofstream outfile_b("b.chtl");
    outfile_b << "[Import] @Chtl from \"a.chtl\";";
    outfile_b.close();

    std::string source = "[Import] @Chtl from \"a.chtl\";";
    std::string expected_path = std::filesystem::weakly_canonical(std::filesystem::current_path() / "a.chtl").string();
    expect_compile_error(source, "Circular import detected: " + expected_path);
}

TEST(ImportTest, NonChtlImports) {
    std::ofstream outfile_html("test.html");
    outfile_html << "<div>Imported HTML</div>";
    outfile_html.close();

    std::ofstream outfile_css("test.css");
    outfile_css << ".imported { color: blue; }";
    outfile_css.close();

    std::ofstream outfile_js("test.js");
    outfile_js << "console.log('imported');";
    outfile_js.close();

    std::string source = R"(
        html {
            head {
                [Import] @Style from "test.css" as myStyles;
            }
            body {
                [Import] @Html from "test.html" as myHtml;
                [Import] @JavaScript from "test.js" as myScript;
            }
        }
    )";

    std::string expected = R"(
        <html>
        <head>
        <style>
        .imported { color: blue; }
        </style>
        </head>
        <body>
        <div>Imported HTML</div>
        <script>
        console.log('imported');
        </script>
        </body>
        </html>
    )";

    EXPECT_EQ(removeWhitespace(compile(source, "main.chtl")), removeWhitespace(expected));
}
