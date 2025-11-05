#include <gtest/gtest.h>
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/StyleNode.h"
#include "CHTL/CHTLNode/ScriptNode.h"

TEST(ParserTest, ParseSingleElement) {
    std::string source = "div {}";
    CHTL::Parser parser(source);
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->children.size(), 1);

    auto element = dynamic_cast<CHTL::ElementNode*>(program->children[0].get());
    ASSERT_NE(element, nullptr);
    EXPECT_EQ(element->tagName, "div");
}

TEST(ParserTest, ParseNestedElements) {
    std::string source = "html { body { div {} } }";
    CHTL::Parser parser(source);
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->children.size(), 1);

    auto html = dynamic_cast<CHTL::ElementNode*>(program->children[0].get());
    ASSERT_NE(html, nullptr);
    EXPECT_EQ(html->tagName, "html");
    ASSERT_EQ(html->children.size(), 1);

    auto body = dynamic_cast<CHTL::ElementNode*>(html->children[0].get());
    ASSERT_NE(body, nullptr);
    EXPECT_EQ(body->tagName, "body");
    ASSERT_EQ(body->children.size(), 1);

    auto div = dynamic_cast<CHTL::ElementNode*>(body->children[0].get());
    ASSERT_NE(div, nullptr);
    EXPECT_EQ(div->tagName, "div");
}

TEST(ParserTest, ParseTextNode) {
    std::string source = "text { \"hello\" }";
    CHTL::Parser parser(source);
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->children.size(), 1);

    auto text = dynamic_cast<CHTL::TextNode*>(program->children[0].get());
    ASSERT_NE(text, nullptr);
    EXPECT_EQ(text->content, "hello");
}

TEST(ParserTest, ParseElementWithText) {
    std::string source = "p { text { \"world\" } }";
    CHTL::Parser parser(source);
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->children.size(), 1);

    auto p = dynamic_cast<CHTL::ElementNode*>(program->children[0].get());
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->tagName, "p");
    ASSERT_EQ(p->children.size(), 1);

    auto text = dynamic_cast<CHTL::TextNode*>(p->children[0].get());
    ASSERT_NE(text, nullptr);
    EXPECT_EQ(text->content, "world");
}

TEST(ParserTest, ParseAttributes) {
    std::string source = "div { id: \"main\"; class = box; }";
    CHTL::Parser parser(source);
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->children.size(), 1);

    auto element = dynamic_cast<CHTL::ElementNode*>(program->children[0].get());
    ASSERT_NE(element, nullptr);
    EXPECT_EQ(element->tagName, "div");

    ASSERT_EQ(element->attributes.size(), 2);
    EXPECT_EQ(element->attributes["id"], "main");
    EXPECT_EQ(element->attributes["class"], "box");
}

// TEST(ParserTest, ParseStyleBlock) {
//     std::string source = "div { style { color: red; } }";
//     CHTL::Parser parser(source);
//     auto program = parser.parse();

//     ASSERT_NE(program, nullptr);
//     ASSERT_EQ(program->children.size(), 1);

//     auto element = dynamic_cast<CHTL::ElementNode*>(program->children[0].get());
//     ASSERT_NE(element, nullptr);
//     ASSERT_EQ(element->children.size(), 1);

//     auto style = dynamic_cast<CHTL::StyleNode*>(element->children[0].get());
//     ASSERT_NE(style, nullptr);
//     EXPECT_EQ(style->content, " color: red; ");
// }

// TEST(ParserTest, ParseScriptBlock) {
//     std::string source = "div { script { console.log(\"hello\"); } }";
//     CHTL::Parser parser(source);
//     auto program = parser.parse();

//     ASSERT_NE(program, nullptr);
//     ASSERT_EQ(program->children.size(), 1);

//     auto element = dynamic_cast<CHTL::ElementNode*>(program->children[0].get());
//     ASSERT_NE(element, nullptr);
//     ASSERT_EQ(element->children.size(), 1);

//     auto script = dynamic_cast<CHTL::ScriptNode*>(element->children[0].get());
//     ASSERT_NE(script, nullptr);
//     EXPECT_EQ(script->content, " console.log(\"hello\"); ");
// }
