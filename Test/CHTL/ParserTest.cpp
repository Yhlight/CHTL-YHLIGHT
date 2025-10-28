#include <gtest/gtest.h>
#include "CHTL/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/TextNode.h"

TEST(ParserTest, ParseElementWithAttribute) {
    std::string source = "div { id: box; }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::shared_ptr<CHTL::BaseNode> root = parser.parse();

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

    std::shared_ptr<CHTL::BaseNode> root = parser.parse();

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getType(), CHTL::NodeType::Text);

    auto textNode = std::static_pointer_cast<CHTL::TextNode>(root);
    EXPECT_EQ(textNode->getText(), "hello");
}

TEST(ParserTest, ParseSimpleElement) {
    std::string source = "div {}";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::shared_ptr<CHTL::BaseNode> root = parser.parse();

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

    std::shared_ptr<CHTL::BaseNode> root = parser.parse();

    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->getType(), CHTL::NodeType::Element);

    auto divNode = std::static_pointer_cast<CHTL::ElementNode>(root);
    EXPECT_EQ(divNode->getTagName(), "div");
    ASSERT_EQ(divNode->getChildren().size(), 1);

    auto spanNode = std::static_pointer_cast<CHTL::ElementNode>(divNode->getChildren()[0]);
    EXPECT_EQ(spanNode->getTagName(), "span");
    EXPECT_TRUE(spanNode->getChildren().empty());
}
