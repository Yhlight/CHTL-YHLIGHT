#include "gtest/gtest.h"
#include "CHTL/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/OriginNode.h"
#include <fstream>
#include <cstdio>
#include <stdexcept>

class ImportTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create dummy files for testing
        std::ofstream html_file("test.html");
        html_file << "<h1>Hello</h1>";
        html_file.close();

        std::ofstream style_file("test.css");
        style_file << "body { color: red; }";
        style_file.close();

        std::ofstream js_file("test.js");
        js_file << "console.log('hello');";
        js_file.close();
    }

    void TearDown() override {
        // Clean up dummy files
        std::remove("test.html");
        std::remove("test.css");
        std::remove("test.js");
    }
};

TEST_F(ImportTest, ParseBasicHtmlImport) {
    std::string source = "[Import] @Html from \"test.html\" as myHtml;";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    parser.parse();

    const auto& originTemplates = parser.getOriginTemplates();
    auto it = originTemplates.find("myHtml");
    ASSERT_NE(it, originTemplates.end());
    EXPECT_EQ(it->second->getContent(), "<h1>Hello</h1>");
    EXPECT_EQ(it->second->getOriginType(), CHTL::OriginType::Html);
}

TEST_F(ImportTest, ParseBasicStyleImport) {
    std::string source = "[Import] @Style from \"test.css\" as myStyle;";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    parser.parse();

    const auto& originTemplates = parser.getOriginTemplates();
    auto it = originTemplates.find("myStyle");
    ASSERT_NE(it, originTemplates.end());
    EXPECT_EQ(it->second->getContent(), "body { color: red; }");
    EXPECT_EQ(it->second->getOriginType(), CHTL::OriginType::Style);
}

TEST_F(ImportTest, ParseBasicJavaScriptImport) {
    std::string source = "[Import] @JavaScript from \"test.js\" as myJs;";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    parser.parse();

    const auto& originTemplates = parser.getOriginTemplates();
    auto it = originTemplates.find("myJs");
    ASSERT_NE(it, originTemplates.end());
    EXPECT_EQ(it->second->getContent(), "console.log('hello');");
    EXPECT_EQ(it->second->getOriginType(), CHTL::OriginType::JavaScript);
}

TEST_F(ImportTest, ThrowsOnChtlImport) {
    std::string source = "[Import] @Chtl from \"test.chtl\" as myChtl;";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    EXPECT_THROW(parser.parse(), std::runtime_error);
}

TEST_F(ImportTest, ThrowsOnUnsupportedImportType) {
    std::string source = "[Import] @Unknown from \"test.txt\" as myTxt;";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    EXPECT_THROW(parser.parse(), std::runtime_error);
}
