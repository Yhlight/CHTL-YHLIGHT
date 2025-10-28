#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLNode/ASTNode.h"

using namespace CHTL;

TEST(ParserTest, ParsesBasicElement) {
    std::string source = "div {}";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->children.size(), 1);

    auto* element = dynamic_cast<ElementNode*>(program->children[0].get());
    ASSERT_NE(element, nullptr);
    EXPECT_EQ(element->tagName, "div");
    EXPECT_TRUE(element->attributes.empty());
    EXPECT_TRUE(element->children.empty());
}

TEST(ParserTest, ParsesNestedElements) {
    std::string source = "html { body { div {} } }";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();

    ASSERT_EQ(program->children.size(), 1);
    auto* html = dynamic_cast<ElementNode*>(program->children[0].get());
    ASSERT_NE(html, nullptr);
    EXPECT_EQ(html->tagName, "html");

    ASSERT_EQ(html->children.size(), 1);
    auto* body = dynamic_cast<ElementNode*>(html->children[0].get());
    ASSERT_NE(body, nullptr);
    EXPECT_EQ(body->tagName, "body");

    ASSERT_EQ(body->children.size(), 1);
    auto* div = dynamic_cast<ElementNode*>(body->children[0].get());
    ASSERT_NE(div, nullptr);
    EXPECT_EQ(div->tagName, "div");
}

TEST(ParserTest, ParsesElementWithAttributes) {
    std::string source = "div { id: box; class: container; }";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();

    ASSERT_EQ(program->children.size(), 1);
    auto* div = dynamic_cast<ElementNode*>(program->children[0].get());
    ASSERT_NE(div, nullptr);

    ASSERT_EQ(div->attributes.size(), 2);
    EXPECT_EQ(div->attributes[0].key, "id");
    EXPECT_EQ(div->attributes[0].value, "box");
    EXPECT_EQ(div->attributes[1].key, "class");
    EXPECT_EQ(div->attributes[1].value, "container");
}

TEST(ParserTest, ParsesTextNode) {
    std::string source = R"(
        p {
            text { "Hello, World!" }
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();

    ASSERT_EQ(program->children.size(), 1);
    auto* p = dynamic_cast<ElementNode*>(program->children[0].get());
    ASSERT_NE(p, nullptr);

    ASSERT_EQ(p->children.size(), 1);
    auto* text = dynamic_cast<TextNode*>(p->children[0].get());
    ASSERT_NE(text, nullptr);
    EXPECT_EQ(text->content, "Hello, World!");
}

TEST(ParserTest, ThrowsOnUnterminatedBlock) {
    std::string source = "div {";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());

    EXPECT_THROW(parser.parse(), std::runtime_error);
}

TEST(ParserTest, ThrowsOnMissingSemicolon) {
    std::string source = "div { id: box }";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());

    EXPECT_THROW(parser.parse(), std::runtime_error);
}
