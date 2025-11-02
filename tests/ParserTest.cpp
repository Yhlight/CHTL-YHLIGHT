#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/ElementNode.h"

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
