#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/StyleNode.h"
#include "CHTL/CHTLNode/StylePropertyNode.h"

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

    CHTL::StylePropertyNode* propNode = styleNode->properties[0].get();
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

    CHTL::StylePropertyNode* prop1 = styleNode->properties[0].get();
    EXPECT_EQ(prop1->name, "color");
    EXPECT_EQ(prop1->value, "red");

    CHTL::StylePropertyNode* prop2 = styleNode->properties[1].get();
    EXPECT_EQ(prop2->name, "font-size");
    EXPECT_EQ(prop2->value, "16px");
}
