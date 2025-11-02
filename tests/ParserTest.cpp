#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/TextNode.h"

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
