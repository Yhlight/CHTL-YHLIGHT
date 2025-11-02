#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/StyleNode.h"
#include "CHTL/CHTLNode/StylePropertyNode.h"
#include "CHTL/CHTLNode/TemplateNode.h"
#include "CHTL/CHTLNode/CustomTemplateNode.h"
#include "CHTL/CHTLNode/OriginNode.h"
#include "CHTL/CHTLNode/ImportNode.h"
#include "CHTL/CHTLNode/NamespaceNode.h"
#include "CHTL/CHTLNode/TemplateUsageNode.h"

TEST(ParserTest, ParsesUnquotedTextBlock) {
    std::string source = "text { hello world }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->getType(), CHTL::ASTNodeType::Program);

    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ASTNode* textNodeBase = programNode->children[0].get();
    ASSERT_EQ(textNodeBase->getType(), CHTL::ASTNodeType::Text);

    CHTL::TextNode* textNode = static_cast<CHTL::TextNode*>(textNodeBase);
    EXPECT_EQ(textNode->content, " hello world ");
}

TEST(ParserTest, ParsesEmptyTextBlock) {
    std::string source = "text {}";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->getType(), CHTL::ASTNodeType::Program);

    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ASTNode* textNodeBase = programNode->children[0].get();
    ASSERT_EQ(textNodeBase->getType(), CHTL::ASTNodeType::Text);

    CHTL::TextNode* textNode = static_cast<CHTL::TextNode*>(textNodeBase);
    EXPECT_EQ(textNode->content, "");
}

TEST(ParserTest, ParsesTextBlockWithWhitespace) {
    std::string source = "text {   hello world   }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->getType(), CHTL::ASTNodeType::Program);

    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ASTNode* textNodeBase = programNode->children[0].get();
    ASSERT_EQ(textNodeBase->getType(), CHTL::ASTNodeType::Text);

    CHTL::TextNode* textNode = static_cast<CHTL::TextNode*>(textNodeBase);
    EXPECT_EQ(textNode->content, "   hello world   ");
}

TEST(ParserTest, ParsesSingleElement) {
    std::string source = "div {}";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ASTNode* elementNodeBase = programNode->children[0].get();
    ASSERT_EQ(elementNodeBase->getType(), CHTL::ASTNodeType::Element);

    CHTL::ElementNode* elementNode = static_cast<CHTL::ElementNode*>(elementNodeBase);
    EXPECT_EQ(elementNode->tagName, "div");
    EXPECT_EQ(elementNode->children.size(), 0);
}

TEST(ParserTest, ParsesNestedElements) {
    std::string source = "div { span {} }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ElementNode* divNode = static_cast<CHTL::ElementNode*>(programNode->children[0].get());
    ASSERT_EQ(divNode->tagName, "div");
    ASSERT_EQ(divNode->children.size(), 1);

    CHTL::ElementNode* spanNode = static_cast<CHTL::ElementNode*>(divNode->children[0].get());
    EXPECT_EQ(spanNode->tagName, "span");
    EXPECT_EQ(spanNode->children.size(), 0);
}

TEST(ParserTest, ParsesElementWithText) {
    std::string source = "div { text { hello } }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ElementNode* divNode = static_cast<CHTL::ElementNode*>(programNode->children[0].get());
    ASSERT_EQ(divNode->tagName, "div");
    ASSERT_EQ(divNode->children.size(), 1);

    CHTL::TextNode* textNode = static_cast<CHTL::TextNode*>(divNode->children[0].get());
    EXPECT_EQ(textNode->content, " hello ");
}

TEST(ParserTest, ParsesSingleAttribute) {
    std::string source = "div { id: \"main\"; }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ElementNode* elementNode = static_cast<CHTL::ElementNode*>(programNode->children[0].get());
    ASSERT_EQ(elementNode->attributes.size(), 1);
    EXPECT_EQ(elementNode->attributes["id"], "\"main\"");
}

TEST(ParserTest, ParsesMultipleAttributes) {
    std::string source = "div { id: \"main\"; class: \"container\"; }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ElementNode* elementNode = static_cast<CHTL::ElementNode*>(programNode->children[0].get());
    ASSERT_EQ(elementNode->attributes.size(), 2);
    EXPECT_EQ(elementNode->attributes["id"], "\"main\"");
    EXPECT_EQ(elementNode->attributes["class"], "\"container\"");
}

TEST(ParserTest, ParsesAttributesAndChildren) {
    std::string source = "div { id: \"main\"; span {} }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ElementNode* divNode = static_cast<CHTL::ElementNode*>(programNode->children[0].get());
    ASSERT_EQ(divNode->attributes.size(), 1);
    EXPECT_EQ(divNode->attributes["id"], "\"main\"");
    ASSERT_EQ(divNode->children.size(), 1);

    CHTL::ElementNode* spanNode = static_cast<CHTL::ElementNode*>(divNode->children[0].get());
    EXPECT_EQ(spanNode->tagName, "span");
}

TEST(ParserTest, ParsesStyleBlock) {
    std::string source = "style { color: red; }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ASTNode* styleNodeBase = programNode->children[0].get();
    ASSERT_EQ(styleNodeBase->getType(), CHTL::ASTNodeType::StyleBlock);

    CHTL::StyleNode* styleNode = static_cast<CHTL::StyleNode*>(styleNodeBase);
    ASSERT_EQ(styleNode->properties.size(), 1);

    CHTL::StylePropertyNode* propNode = static_cast<CHTL::StylePropertyNode*>(styleNode->properties[0].get());
    EXPECT_EQ(propNode->name, "color");
    EXPECT_EQ(propNode->value, "red");
}

TEST(ParserTest, ParsesMultipleStyleProperties) {
    std::string source = "style { color: red; font-size: 16px; }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::StyleNode* styleNode = static_cast<CHTL::StyleNode*>(programNode->children[0].get());
    ASSERT_EQ(styleNode->properties.size(), 2);

    CHTL::StylePropertyNode* prop1 = static_cast<CHTL::StylePropertyNode*>(styleNode->properties[0].get());
    EXPECT_EQ(prop1->name, "color");
    EXPECT_EQ(prop1->value, "red");

    CHTL::StylePropertyNode* prop2 = static_cast<CHTL::StylePropertyNode*>(styleNode->properties[1].get());
    EXPECT_EQ(prop2->name, "font-size");
    EXPECT_EQ(prop2->value, "16px");
}

TEST(ParserTest, ParsesStyleTemplate) {
    std::string source = "[Template] @Style MyStyle { color: red; }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ASTNode* templateNodeBase = programNode->children[0].get();
    ASSERT_EQ(templateNodeBase->getType(), CHTL::ASTNodeType::Template);

    CHTL::TemplateNode* templateNode = static_cast<CHTL::TemplateNode*>(templateNodeBase);
    EXPECT_EQ(templateNode->templateType, CHTL::TemplateType::Style);
    EXPECT_EQ(templateNode->name, "MyStyle");
    ASSERT_EQ(templateNode->children.size(), 1);

    CHTL::StyleNode* styleNode = static_cast<CHTL::StyleNode*>(templateNode->children[0].get());
    ASSERT_EQ(styleNode->properties.size(), 1);

    CHTL::StylePropertyNode* propNode = static_cast<CHTL::StylePropertyNode*>(styleNode->properties[0].get());
    EXPECT_EQ(propNode->name, "color");
    EXPECT_EQ(propNode->value, "red");
}

TEST(ParserTest, ParsesElementTemplate) {
    std::string source = "[Template] @Element MyElement { div {} }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::TemplateNode* templateNode = static_cast<CHTL::TemplateNode*>(programNode->children[0].get());
    EXPECT_EQ(templateNode->templateType, CHTL::TemplateType::Element);
    EXPECT_EQ(templateNode->name, "MyElement");
    ASSERT_EQ(templateNode->children.size(), 1);
}

TEST(ParserTest, ParsesCustomStyleTemplate) {
    std::string source = "[Custom] @Style MyStyle { color: red; }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ASTNode* customTemplateNodeBase = programNode->children[0].get();
    ASSERT_EQ(customTemplateNodeBase->getType(), CHTL::ASTNodeType::CustomTemplate);

    CHTL::CustomTemplateNode* customTemplateNode = static_cast<CHTL::CustomTemplateNode*>(customTemplateNodeBase);
    EXPECT_EQ(customTemplateNode->templateType, CHTL::TemplateType::Style);
    EXPECT_EQ(customTemplateNode->name, "MyStyle");
    ASSERT_EQ(customTemplateNode->children.size(), 1);
}

TEST(ParserTest, ParsesStylePlaceholder) {
    std::string source = "[Custom] @Style MyStyle { color; }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::CustomTemplateNode* customTemplateNode = static_cast<CHTL::CustomTemplateNode*>(programNode->children[0].get());
    ASSERT_EQ(customTemplateNode->children.size(), 1);

    CHTL::StyleNode* styleNode = static_cast<CHTL::StyleNode*>(customTemplateNode->children[0].get());
    ASSERT_EQ(styleNode->properties.size(), 1);

    CHTL::StylePropertyNode* propNode = static_cast<CHTL::StylePropertyNode*>(styleNode->properties[0].get());
    EXPECT_EQ(propNode->name, "color");
    EXPECT_FALSE(propNode->value.has_value());
}

TEST(ParserTest, ParsesAnonymousOriginBlock) {
    std::string source = "[Origin] @Html { <div>hello</div> }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ASTNode* originNodeBase = programNode->children[0].get();
    ASSERT_EQ(originNodeBase->getType(), CHTL::ASTNodeType::Origin);

    CHTL::OriginNode* originNode = static_cast<CHTL::OriginNode*>(originNodeBase);
    EXPECT_EQ(originNode->originType, "@Html");
    EXPECT_FALSE(originNode->name.has_value());
    EXPECT_EQ(originNode->content, " <div>hello</div> ");
}

TEST(ParserTest, ParsesNamedOriginBlock) {
    std::string source = "[Origin] @JavaScript myScript { console.log('hello'); }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::OriginNode* originNode = static_cast<CHTL::OriginNode*>(programNode->children[0].get());
    EXPECT_EQ(originNode->originType, "@JavaScript");
    ASSERT_TRUE(originNode->name.has_value());
    EXPECT_EQ(originNode->name.value(), "myScript");
    EXPECT_EQ(originNode->content, " console.log('hello'); ");
}

TEST(ParserTest, ParsesSimpleImport) {
    std::string source = "[Import] @Chtl from \"./myFile.chtl\"";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ASTNode* importNodeBase = programNode->children[0].get();
    ASSERT_EQ(importNodeBase->getType(), CHTL::ASTNodeType::Import);

    CHTL::ImportNode* importNode = static_cast<CHTL::ImportNode*>(importNodeBase);
    ASSERT_EQ(importNode->targets.size(), 1);
    EXPECT_EQ(importNode->targets[0].type, "@Chtl");
    EXPECT_EQ(importNode->path, "\"./myFile.chtl\"");
    EXPECT_FALSE(importNode->alias.has_value());
}

TEST(ParserTest, ParsesImportWithAlias) {
    std::string source = "[Import] @Element MyElement from \"./elements.chtl\" as MyAlias";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ImportNode* importNode = static_cast<CHTL::ImportNode*>(programNode->children[0].get());
    ASSERT_EQ(importNode->targets.size(), 1);
    EXPECT_EQ(importNode->targets[0].type, "@Element");
    ASSERT_TRUE(importNode->targets[0].name.has_value());
    EXPECT_EQ(importNode->targets[0].name.value(), "MyElement");
    EXPECT_EQ(importNode->path, "\"./elements.chtl\"");
    ASSERT_TRUE(importNode->alias.has_value());
    EXPECT_EQ(importNode->alias.value(), "MyAlias");
}

TEST(ParserTest, ParsesMultiTokenStyleValue) {
    std::string source = "style { font-family: \"Times New Roman\", serif; }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::StyleNode* styleNode = static_cast<CHTL::StyleNode*>(programNode->children[0].get());
    ASSERT_EQ(styleNode->properties.size(), 1);

    CHTL::StylePropertyNode* prop = static_cast<CHTL::StylePropertyNode*>(styleNode->properties[0].get());
    EXPECT_EQ(prop->name, "font-family");
    EXPECT_EQ(prop->value.value(), "\"Times New Roman\", serif");
}

TEST(ParserTest, ParsesMultiTokenAttributeValue) {
    std::string source = "div { class: \"btn btn-primary\"; }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ElementNode* elementNode = static_cast<CHTL::ElementNode*>(programNode->children[0].get());
    ASSERT_EQ(elementNode->attributes.size(), 1);
    EXPECT_EQ(elementNode->attributes["class"], "\"btn btn-primary\"");
}

TEST(ParserTest, ParsesNamespaceBlock) {
    std::string source = "[Namespace] my_space { div {} }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ASTNode* namespaceNodeBase = programNode->children[0].get();
    ASSERT_EQ(namespaceNodeBase->getType(), CHTL::ASTNodeType::Namespace);

    CHTL::NamespaceNode* namespaceNode = static_cast<CHTL::NamespaceNode*>(namespaceNodeBase);
    EXPECT_EQ(namespaceNode->name, "my_space");
    ASSERT_EQ(namespaceNode->children.size(), 1);

    CHTL::ElementNode* elementNode = static_cast<CHTL::ElementNode*>(namespaceNode->children[0].get());
    EXPECT_EQ(elementNode->tagName, "div");
}

TEST(ParserTest, ParsesStyleTemplateUsage) {
    std::string source = "style { @Style MyTheme; }";
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    std::unique_ptr<CHTL::ASTNode> ast = parser.parse();

    ASSERT_NE(ast, nullptr);
    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::StyleNode* styleNode = static_cast<CHTL::StyleNode*>(programNode->children[0].get());
    ASSERT_EQ(styleNode->properties.size(), 1);

    CHTL::ASTNode* templateUsageBase = styleNode->properties[0].get();
    ASSERT_EQ(templateUsageBase->getType(), CHTL::ASTNodeType::TemplateUsage);

    CHTL::TemplateUsageNode* templateUsageNode = static_cast<CHTL::TemplateUsageNode*>(templateUsageBase);
    EXPECT_EQ(templateUsageNode->name, "MyTheme");
}
