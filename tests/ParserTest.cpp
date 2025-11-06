#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/ElementNode.h"

TEST(ParserTest, ParseTextNode) {
    std::string source = "text { \"hello world\" }";
    Lexer lexer(source);
    Parser parser(lexer);

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    const auto& children = program->getChildren();
    ASSERT_EQ(children.size(), 1);

    const auto& child = children[0];
    ASSERT_EQ(child->getType(), ASTNodeType::Text);

    auto* textNode = static_cast<TextNode*>(child.get());
    EXPECT_EQ(textNode->getText(), "hello world");
}

TEST(ParserTest, ParseSingleElementNode) {
    std::string source = "div {}";
    Lexer lexer(source);
    Parser parser(lexer);

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    const auto& children = program->getChildren();
    ASSERT_EQ(children.size(), 1);

    const auto& child = children[0];
    ASSERT_EQ(child->getType(), ASTNodeType::Element);

    auto* elementNode = static_cast<ElementNode*>(child.get());
    EXPECT_EQ(elementNode->getTagName(), "div");
}

TEST(ParserTest, ParseElementWithAttributes) {
    std::string source = "div { id: \"main\"; class: container; }";
    Lexer lexer(source);
    Parser parser(lexer);

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    const auto& children = program->getChildren();
    ASSERT_EQ(children.size(), 1);

    const auto& child = children[0];
    ASSERT_EQ(child->getType(), ASTNodeType::Element);

    auto* elementNode = static_cast<ElementNode*>(child.get());
    EXPECT_EQ(elementNode->getTagName(), "div");

    const auto& attributes = elementNode->getAttributes();
    EXPECT_EQ(attributes.size(), 2);
    EXPECT_EQ(attributes.at("id"), "main");
    EXPECT_EQ(attributes.at("class"), "container");
}

TEST(ParserTest, ParseNestedElements) {
    std::string source = "body { div { p { text { \"nested\" } } } }";
    Lexer lexer(source);
    Parser parser(lexer);

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    const auto& body_children = program->getChildren();
    ASSERT_EQ(body_children.size(), 1);
    auto* bodyNode = static_cast<ElementNode*>(body_children[0].get());
    EXPECT_EQ(bodyNode->getTagName(), "body");

    const auto& div_children = bodyNode->getChildren();
    ASSERT_EQ(div_children.size(), 1);
    auto* divNode = static_cast<ElementNode*>(div_children[0].get());
    EXPECT_EQ(divNode->getTagName(), "div");

    const auto& p_children = divNode->getChildren();
    ASSERT_EQ(p_children.size(), 1);
    auto* pNode = static_cast<ElementNode*>(p_children[0].get());
    EXPECT_EQ(pNode->getTagName(), "p");

    const auto& text_children = pNode->getChildren();
    ASSERT_EQ(text_children.size(), 1);
    auto* textNode = static_cast<TextNode*>(text_children[0].get());
    EXPECT_EQ(textNode->getText(), "nested");
}
