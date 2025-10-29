#include "gtest/gtest.h"
#include "CHTL/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/ImportNode.h"
#include "CHTL/CHTLNode/TemplateElementDefinitionNode.h"
#include <fstream>
#include <stdexcept>

class ImportTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create dummy files for testing imports
        std::ofstream("test_import_element.chtl") << "[Template] @Element MyElement {}";
        std::ofstream("test_import_category.chtl") << "[Template] @Element El1 {} [Template] @Style St1 {}";
        std::ofstream("test_import_style.css") << ".red { color: red; }";
    }

    void TearDown() override {
        // Clean up dummy files
        std::remove("test_import_element.chtl");
        std::remove("test_import_category.chtl");
        std::remove("test_import_style.css");
    }
};

TEST_F(ImportTest, CanImportSpecificElement) {
    std::string chtlCode = "[Import] [Template] @Element MyElement from \"test_import_element.chtl\"";
    CHTL::Lexer lexer(chtlCode);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    ASSERT_EQ(parser.getElementTemplates().count("MyElement"), 1);
}

TEST_F(ImportTest, CanImportAllTemplates) {
    std::string chtlCode = "[Import] [Template] from \"test_import_category.chtl\"";
    CHTL::Lexer lexer(chtlCode);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    ASSERT_EQ(parser.getElementTemplates().count("El1"), 1);
    ASSERT_EQ(parser.getStyleTemplates().count("St1"), 1);
}

TEST_F(ImportTest, CanImportCssAsOrigin) {
    std::string chtlCode = "[Import] @Style from \"test_import_style.css\" as MyStyle";
    CHTL::Lexer lexer(chtlCode);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    ASSERT_EQ(parser.getOriginTemplates().count("MyStyle"), 1);
    auto originNode = parser.getOriginTemplates().at("MyStyle");
    ASSERT_EQ(originNode->getContent(), ".red { color: red; }");
}

TEST_F(ImportTest, ThrowsErrorOnMissingFile) {
    std::string chtlCode = "[Import] @Chtl from \"non_existent_file.chtl\"";
    CHTL::Lexer lexer(chtlCode);
    CHTL::Parser parser(lexer);
    ASSERT_THROW(parser.parse(), std::runtime_error);
}
