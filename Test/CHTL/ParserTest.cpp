#include <gtest/gtest.h>
#include "CHTL/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/TemplateStyleDefinitionNode.h"
#include "CHTL/CHTLNode/TemplateStyleUsageNode.h"
#include "CHTL/CHTLNode/TemplateElementDefinitionNode.h"
#include "CHTL/CHTLNode/TemplateElementUsageNode.h"
#include "CHTL/CHTLNode/TemplateVarDefinitionNode.h"
#include "CHTL/CHTLNode/StylePropertyNode.h"
#include "CHTL/CHTLNode/OriginNode.h"

TEST(ParserTest, ParseNamedOriginBlock) {
    std::string source = "[Origin] @Html MyRawHtml {<div></div>}";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    auto programNode = parser.parse();
    ASSERT_NE(programNode, nullptr);
    ASSERT_EQ(programNode->getChildren().size(), 1);

    auto definition = programNode->getChildren()[0];
    ASSERT_NE(definition, nullptr);
    ASSERT_EQ(definition->getType(), CHTL::NodeType::Origin);
    auto originNode = std::static_pointer_cast<CHTL::OriginNode>(definition);
    EXPECT_EQ(originNode->getName().value_or(""), "MyRawHtml");
    EXPECT_EQ(originNode->getContent(), "<div></div>");
}

TEST(ParserTest, ParseOriginBlock) {
    std::string rawContent = "\n    <div class=\"raw-html\">This is not parsed.</div>\n    // Even comments are preserved.\n";
    std::string source = "[Origin] @Html {" + rawContent + "}";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    auto programNode = parser.parse();
    ASSERT_NE(programNode, nullptr);
    ASSERT_EQ(programNode->getChildren().size(), 1);
    auto root = programNode->getChildren()[0];

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getType(), CHTL::NodeType::Origin);

    auto originNode = std::static_pointer_cast<CHTL::OriginNode>(root);
    EXPECT_EQ(originNode->getOriginType(), CHTL::OriginType::Html);
    EXPECT_EQ(originNode->getContent(), rawContent);
}

TEST(ParserTest, ParseTemplateVarDefinition) {
    std::string source = "[Template] @Var MyVars { color: red; }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    auto programNode = parser.parse();
    ASSERT_NE(programNode, nullptr);
    ASSERT_EQ(programNode->getChildren().size(), 1);
    auto root = programNode->getChildren()[0];

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getType(), CHTL::NodeType::TemplateVarDefinition);

    auto templateNode = std::static_pointer_cast<CHTL::TemplateVarDefinitionNode>(root);
    EXPECT_EQ(templateNode->getName(), "MyVars");
    ASSERT_EQ(templateNode->getVariables().size(), 1);
    EXPECT_EQ(templateNode->getVariables().at("color"), "red");
}

TEST(ParserTest, ParseStructuredStyleBlock) {
    std::string source = "div { style { color: MyVars(color); font-size: 16px; } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    auto programNode = parser.parse();
    ASSERT_NE(programNode, nullptr);
    ASSERT_EQ(programNode->getChildren().size(), 1);
    auto root = programNode->getChildren()[0];

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getType(), CHTL::NodeType::Element);

    auto elementNode = std::static_pointer_cast<CHTL::ElementNode>(root);
    auto styleBlock = elementNode->getStyleBlock();
    ASSERT_NE(styleBlock, nullptr);

    ASSERT_EQ(styleBlock->getProperties().size(), 2);
    EXPECT_EQ(styleBlock->getProperties()[0]->getKey(), "color");
    EXPECT_EQ(styleBlock->getProperties()[0]->getValue(), "MyVars(color)");
    EXPECT_EQ(styleBlock->getProperties()[1]->getKey(), "font-size");
    EXPECT_EQ(styleBlock->getProperties()[1]->getValue(), "16px");
}


TEST(ParserTest, ParseTemplateElementDefinition) {
    std::string source = "[Template] @Element MyElement { span {} }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    auto programNode = parser.parse();
    ASSERT_NE(programNode, nullptr);
    ASSERT_EQ(programNode->getChildren().size(), 1);
    auto root = programNode->getChildren()[0];

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getType(), CHTL::NodeType::TemplateElementDefinition);

    auto templateNode = std::static_pointer_cast<CHTL::TemplateElementDefinitionNode>(root);
    EXPECT_EQ(templateNode->getName(), "MyElement");
    ASSERT_EQ(templateNode->getChildren().size(), 1);

    auto child = std::static_pointer_cast<CHTL::ElementNode>(templateNode->getChildren()[0]);
    EXPECT_EQ(child->getTagName(), "span");
}

TEST(ParserTest, ParseElementWithTemplateUsage) {
    std::string source = "div { @Element MyElement; }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    auto programNode = parser.parse();
    ASSERT_NE(programNode, nullptr);
    ASSERT_EQ(programNode->getChildren().size(), 1);
    auto root = programNode->getChildren()[0];

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getType(), CHTL::NodeType::Element);

    auto elementNode = std::static_pointer_cast<CHTL::ElementNode>(root);
    EXPECT_EQ(elementNode->getTagName(), "div");
    ASSERT_EQ(elementNode->getChildren().size(), 1);

    auto usageNode = std::static_pointer_cast<CHTL::TemplateElementUsageNode>(elementNode->getChildren()[0]);
    EXPECT_EQ(usageNode->getName(), "MyElement");
}

TEST(ParserTest, ParseElementWithStyleBlockAndTemplateUsage) {
    std::string source = "div { style { @Style MyStyles; color: blue; } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    auto programNode = parser.parse();
    ASSERT_NE(programNode, nullptr);
    ASSERT_EQ(programNode->getChildren().size(), 1);
    auto root = programNode->getChildren()[0];

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getType(), CHTL::NodeType::Element);

    auto elementNode = std::static_pointer_cast<CHTL::ElementNode>(root);
    EXPECT_EQ(elementNode->getTagName(), "div");

    auto styleBlock = elementNode->getStyleBlock();
    ASSERT_NE(styleBlock, nullptr);

    ASSERT_EQ(styleBlock->getUsedTemplates().size(), 1);
    EXPECT_EQ(styleBlock->getUsedTemplates()[0], "MyStyles");

    ASSERT_EQ(styleBlock->getProperties().size(), 1);
    EXPECT_EQ(styleBlock->getProperties()[0]->getKey(), "color");
    EXPECT_EQ(styleBlock->getProperties()[0]->getValue(), "blue");
}

TEST(ParserTest, ParseTemplateStyleDefinition) {
    std::string source = "[Template] @Style MyStyles { color: red; }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    auto programNode = parser.parse();
    ASSERT_NE(programNode, nullptr);
    ASSERT_EQ(programNode->getChildren().size(), 1);
    auto root = programNode->getChildren()[0];

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getType(), CHTL::NodeType::TemplateStyleDefinition);

    auto templateNode = std::static_pointer_cast<CHTL::TemplateStyleDefinitionNode>(root);
    EXPECT_EQ(templateNode->getName(), "MyStyles");

    auto styleBlock = templateNode->getStyleBlock();
    ASSERT_NE(styleBlock, nullptr);

    ASSERT_EQ(styleBlock->getProperties().size(), 1);
    EXPECT_EQ(styleBlock->getProperties()[0]->getKey(), "color");
    EXPECT_EQ(styleBlock->getProperties()[0]->getValue(), "red");
}

TEST(ParserTest, ParseElementWithTextAttribute) {
    std::string source = "div { text: \"hello\"; }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    auto programNode = parser.parse();
    ASSERT_NE(programNode, nullptr);
    ASSERT_EQ(programNode->getChildren().size(), 1);
    auto root = programNode->getChildren()[0];

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getType(), CHTL::NodeType::Element);

    auto elementNode = std::static_pointer_cast<CHTL::ElementNode>(root);
    EXPECT_EQ(elementNode->getTagName(), "div");
    EXPECT_TRUE(elementNode->getAttributes().empty());
    ASSERT_EQ(elementNode->getChildren().size(), 1);

    auto textNode = std::static_pointer_cast<CHTL::TextNode>(elementNode->getChildren()[0]);
    EXPECT_EQ(textNode->getText(), "hello");
}

TEST(ParserTest, ParseElementWithAttribute) {
    std::string source = "div { id: box; }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    auto programNode = parser.parse();
    ASSERT_NE(programNode, nullptr);
    ASSERT_EQ(programNode->getChildren().size(), 1);
    auto root = programNode->getChildren()[0];

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getType(), CHTL::NodeType::Element);

    auto elementNode = std::static_pointer_cast<CHTL::ElementNode>(root);
    EXPECT_EQ(elementNode->getTagName(), "div");
    ASSERT_EQ(elementNode->getAttributes().size(), 1);

    auto attribute = elementNode->getAttributes()[0];
    EXPECT_EQ(attribute->getKey(), "id");
    EXPECT_EQ(attribute->getValue(), "box");
}

TEST(ParserTest, ParseTextNode) {
    std::string source = "text { \"hello\" }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    auto programNode = parser.parse();
    ASSERT_NE(programNode, nullptr);
    ASSERT_EQ(programNode->getChildren().size(), 1);
    auto root = programNode->getChildren()[0];

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getType(), CHTL::NodeType::Text);

    auto textNode = std::static_pointer_cast<CHTL::TextNode>(root);
    EXPECT_EQ(textNode->getText(), "hello");
}

TEST(ParserTest, ParseSimpleElement) {
    std::string source = "div {}";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    auto programNode = parser.parse();
    ASSERT_NE(programNode, nullptr);
    ASSERT_EQ(programNode->getChildren().size(), 1);
    auto root = programNode->getChildren()[0];

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getType(), CHTL::NodeType::Element);

    auto elementNode = std::static_pointer_cast<CHTL::ElementNode>(root);
    EXPECT_EQ(elementNode->getTagName(), "div");
    EXPECT_TRUE(elementNode->getChildren().empty());
}

TEST(ParserTest, ParseNestedElement) {
    std::string source = "div { span {} }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    auto programNode = parser.parse();
    ASSERT_NE(programNode, nullptr);
    ASSERT_EQ(programNode->getChildren().size(), 1);
    auto root = programNode->getChildren()[0];

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getType(), CHTL::NodeType::Element);

    auto divNode = std::static_pointer_cast<CHTL::ElementNode>(root);
    EXPECT_EQ(divNode->getTagName(), "div");
    ASSERT_EQ(divNode->getChildren().size(), 1);

    auto spanNode = std::static_pointer_cast<CHTL::ElementNode>(divNode->getChildren()[0]);
    EXPECT_EQ(spanNode->getTagName(), "span");
    EXPECT_TRUE(spanNode->getChildren().empty());
}

TEST(ParserTest, ParseOriginBlockInsideElement) {
    std::string source = "div { [Origin] @Html {<p>raw</p>} }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    auto programNode = parser.parse();
    ASSERT_NE(programNode, nullptr);
    ASSERT_EQ(programNode->getChildren().size(), 1);

    auto divNode = std::static_pointer_cast<CHTL::ElementNode>(programNode->getChildren()[0]);
    ASSERT_NE(divNode, nullptr);
    ASSERT_EQ(divNode->getTagName(), "div");
    ASSERT_EQ(divNode->getChildren().size(), 1);

    auto originNode = std::static_pointer_cast<CHTL::OriginNode>(divNode->getChildren()[0]);
    ASSERT_NE(originNode, nullptr);
    EXPECT_EQ(originNode->getOriginType(), CHTL::OriginType::Html);
    EXPECT_EQ(originNode->getContent(), "<p>raw</p>");
}

TEST(ParserTest, ParseImportStatement) {
    std::string source = "[Import] @Html from \"../../Test/CHTL/resources/test.html\" as MyHtml";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    auto programNode = parser.parse();
    ASSERT_NE(programNode, nullptr);
    ASSERT_EQ(programNode->getChildren().size(), 1);

    auto importNode = std::static_pointer_cast<CHTL::OriginNode>(programNode->getChildren()[0]);
    ASSERT_NE(importNode, nullptr);
    EXPECT_EQ(importNode->getOriginType(), CHTL::OriginType::Html);
    EXPECT_EQ(importNode->getName().value_or(""), "MyHtml");
    EXPECT_EQ(importNode->getContent(), "<p>This is a test file.</p>\n");
}

TEST(ParserTest, ParseImportStatementFileNotFound) {
    std::string source = "[Import] @Html from \"nonexistent.html\" as MyHtml";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    EXPECT_THROW(parser.parse(), std::runtime_error);
}
