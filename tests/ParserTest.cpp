#include "gtest/gtest.h"
#include "Parser.h"
#include "ElementNode.h"
#include "TextNode.h"
#include "Lexer.h"

TEST(ParserTest, EmptyInput) {
    std::vector<Token> tokens;
    Parser parser(tokens);
    auto ast = parser.parse();
    EXPECT_EQ(ast, nullptr);
}

TEST(ParserTest, SimpleElement) {
    Lexer lexer("div { text: \"hello\"; }");
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto ast = parser.parse();
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->getType(), NodeType::Element);
    auto element_node = static_cast<ElementNode*>(ast.get());
    EXPECT_EQ(element_node->tag_name, "div");
    ASSERT_EQ(element_node->children.size(), 1);
    auto text_node = dynamic_cast<TextNode*>(element_node->children[0].get());
    ASSERT_NE(text_node, nullptr);
    EXPECT_EQ(text_node->text, "hello");
}

TEST(ParserTest, ElementWithAttributes) {
    Lexer lexer("div { id: \"main\"; class = \"container\"; }");
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto ast = parser.parse();
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->getType(), NodeType::Element);
    auto element_node = static_cast<ElementNode*>(ast.get());
    EXPECT_EQ(element_node->tag_name, "div");
    EXPECT_EQ(element_node->attributes.size(), 2);
    EXPECT_EQ(element_node->attributes["id"], "main");
    EXPECT_EQ(element_node->attributes["class"], "container");
}
