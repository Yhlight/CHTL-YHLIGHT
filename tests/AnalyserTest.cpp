#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLAnalyser/Analyser.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/StyleNode.h"
#include "CHTL/CHTLNode/StylePropertyNode.h"
#include "CHTL/CHTLNode/TemplateUsageNode.h"

TEST(AnalyserTest, ResolvesStyleTemplate) {
    std::string source =
        "[Template] @Style MyTheme {"
        "    color: red;"
        "}"
        "div {"
        "    style {"
        "        @Style MyTheme;"
        "    }"
        "}";
    CHTL::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    auto ast = parser.parse();
    std::vector<std::string> importStack;
    CHTL::Analyser analyser(*ast, "test.chtl", importStack);
    analyser.analyse();

    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ElementNode* elementNode = static_cast<CHTL::ElementNode*>(programNode->children[0].get());
    ASSERT_EQ(elementNode->children.size(), 1);

    CHTL::StyleNode* styleNode = static_cast<CHTL::StyleNode*>(elementNode->children[0].get());
    ASSERT_EQ(styleNode->properties.size(), 1);

    CHTL::ASTNode* propBase = styleNode->properties[0].get();
    ASSERT_EQ(propBase->getType(), CHTL::ASTNodeType::StyleProperty);

    CHTL::StylePropertyNode* propNode = static_cast<CHTL::StylePropertyNode*>(propBase);
    EXPECT_EQ(propNode->name, "color");
    EXPECT_EQ(propNode->value, "red");
}

TEST(AnalyserTest, ResolvesCustomStyleTemplateWithSpecialization) {
    std::string source =
        "[Custom] @Style MyTheme {"
        "    color;"
        "    font-size: 16px;"
        "}"
        "div {"
        "    style {"
        "        @Style MyTheme {"
        "            color: blue;"
        "        }"
        "    }"
        "}";
    CHTL::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    auto ast = parser.parse();
    std::vector<std::string> importStack;
    CHTL::Analyser analyser(*ast, "test.chtl", importStack);
    analyser.analyse();

    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ElementNode* elementNode = static_cast<CHTL::ElementNode*>(programNode->children[0].get());
    ASSERT_EQ(elementNode->children.size(), 1);

    CHTL::StyleNode* styleNode = static_cast<CHTL::StyleNode*>(elementNode->children[0].get());
    ASSERT_EQ(styleNode->properties.size(), 2);

    CHTL::StylePropertyNode* prop1 = static_cast<CHTL::StylePropertyNode*>(styleNode->properties[0].get());
    EXPECT_EQ(prop1->name, "color");
    EXPECT_EQ(prop1->value, "blue");

    CHTL::StylePropertyNode* prop2 = static_cast<CHTL::StylePropertyNode*>(styleNode->properties[1].get());
    EXPECT_EQ(prop2->name, "font-size");
    EXPECT_EQ(prop2->value, "16px");
}

TEST(AnalyserTest, ResolvesImportedStyleTemplate) {
    std::ofstream importFile("_test_import.chtl");
    importFile << "[Template] @Style MyTheme { color: purple; }";
    importFile.close();

    std::string source =
        "[Import] @Chtl from \"_test_import.chtl\";"
        "div {"
        "    style {"
        "        @Style MyTheme;"
        "    }"
        "}";
    CHTL::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    auto ast = parser.parse();
    std::vector<std::string> importStack;
    CHTL::Analyser analyser(*ast, "test.chtl", importStack);
    analyser.analyse();

    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 2); // Import and div

    CHTL::ElementNode* elementNode = static_cast<CHTL::ElementNode*>(programNode->children[1].get());
    ASSERT_EQ(elementNode->children.size(), 1);

    CHTL::StyleNode* styleNode = static_cast<CHTL::StyleNode*>(elementNode->children[0].get());
    ASSERT_EQ(styleNode->properties.size(), 1);

    CHTL::StylePropertyNode* propNode = static_cast<CHTL::StylePropertyNode*>(styleNode->properties[0].get());
    EXPECT_EQ(propNode->name, "color");
    EXPECT_EQ(propNode->value, "purple");

    std::filesystem::remove("_test_import.chtl");
}

TEST(AnalyserTest, DetectsCircularImports) {
    std::ofstream importFileA("_test_import_A.chtl");
    importFileA << "[Import] @Chtl from \"_test_import_B.chtl\";";
    importFileA.close();

    std::ofstream importFileB("_test_import_B.chtl");
    importFileB << "[Import] @Chtl from \"_test_import_A.chtl\";";
    importFileB.close();

    std::string source = "[Import] @Chtl from \"_test_import_A.chtl\";";
    CHTL::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    auto ast = parser.parse();
    std::vector<std::string> importStack;
    CHTL::Analyser analyser(*ast, "test.chtl", importStack);

    EXPECT_THROW(analyser.analyse(), std::runtime_error);

    std::filesystem::remove("_test_import_A.chtl");
    std::filesystem::remove("_test_import_B.chtl");
}
