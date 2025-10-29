#include "gtest/gtest.h"
#include "CHTL/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLNode/ImportNode.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include <memory>
#include <fstream>

class ImportTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create dummy files for testing
        std::ofstream htmlFile("test.html");
        htmlFile << "<h1>Test</h1>";
        htmlFile.close();

        std::ofstream cssFile("test.css");
        cssFile << "body { color: red; }";
        cssFile.close();

        std::ofstream chtlFile("test.chtl");
        chtlFile << "[Template] @Style MyStyle { color: blue; }";
        chtlFile.close();
    }

    void TearDown() override {
        // Clean up the dummy files
        remove("test.html");
        remove("test.css");
        remove("test.chtl");
    }
};

TEST_F(ImportTest, HandlesHtmlImport) {
    std::string source = "[Import] @Html from \"test.html\" as myHtml";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    ASSERT_EQ(program->getChildren().size(), 1);
    auto importNode = std::dynamic_pointer_cast<CHTL::ImportNode>(program->getChildren()[0]);
    ASSERT_NE(importNode, nullptr);
    EXPECT_EQ(importNode->getImportType(), CHTL::ImportType::Html);
    EXPECT_EQ(importNode->getFilePath(), "test.html");
    ASSERT_TRUE(importNode->getAsName().has_value());
    EXPECT_EQ(importNode->getAsName().value(), "myHtml");

    auto originTemplates = parser.getOriginTemplates();
    ASSERT_EQ(originTemplates.size(), 1);
    auto originNode = originTemplates["myHtml"];
    ASSERT_NE(originNode, nullptr);
    EXPECT_EQ(originNode->getContent(), "<h1>Test</h1>");
}

TEST_F(ImportTest, HandlesCssImport) {
    std::string source = "[Import] @Style from \"test.css\" as myStyle";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    ASSERT_EQ(program->getChildren().size(), 1);
    auto importNode = std::dynamic_pointer_cast<CHTL::ImportNode>(program->getChildren()[0]);
    ASSERT_NE(importNode, nullptr);
    EXPECT_EQ(importNode->getImportType(), CHTL::ImportType::Style);
    EXPECT_EQ(importNode->getFilePath(), "test.css");
    ASSERT_TRUE(importNode->getAsName().has_value());
    EXPECT_EQ(importNode->getAsName().value(), "myStyle");

    auto originTemplates = parser.getOriginTemplates();
    ASSERT_EQ(originTemplates.size(), 1);
    auto originNode = originTemplates["myStyle"];
    ASSERT_NE(originNode, nullptr);
    EXPECT_EQ(originNode->getContent(), "body { color: red; }");
}

TEST_F(ImportTest, HandlesChtlImport) {
    std::string source = "[Import] @Chtl from \"test.chtl\"";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    parser.parse();

    auto styleTemplates = parser.getStyleTemplates();
    ASSERT_EQ(styleTemplates.size(), 1);
    auto styleTemplate = styleTemplates["MyStyle"];
    ASSERT_NE(styleTemplate, nullptr);
}
