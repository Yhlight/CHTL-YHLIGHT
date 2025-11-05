#include "gtest/gtest.h"
#include "Parser.h"
#include "ProgramNode.h"
#include "ElementNode.h"
#include "TextNode.h"
#include "StyleNode.h"
#include "StylePropertyNode.h"
#include "StyleRuleNode.h"
#include "ScriptNode.h"
#include "TemplateNode.h"
#include "ElementDirectiveNode.h"
#include "StyleDirectiveNode.h"
#include "Lexer.h"

TEST(ParserTest, EmptyInput) {
    std::string source = "";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    Parser parser(source, tokens);
    auto ast = parser.parse();
    EXPECT_EQ(ast->children.size(), 0);
}

TEST(ParserTest, SimpleElement) {
    std::string source = "div { text: \"hello\"; }";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    Parser parser(source, tokens);
    auto ast = parser.parse();
    ASSERT_EQ(ast->children.size(), 1);
    auto element_node = dynamic_cast<ElementNode*>(ast->children[0].get());
    ASSERT_NE(element_node, nullptr);
    EXPECT_EQ(element_node->tag_name, "div");
    ASSERT_EQ(element_node->children.size(), 1);
    auto text_node = dynamic_cast<TextNode*>(element_node->children[0].get());
    ASSERT_NE(text_node, nullptr);
    EXPECT_EQ(text_node->text, "hello");
}

TEST(ParserTest, ElementWithAttributes) {
    std::string source = "div { id: \"main\"; class = \"container\"; }";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    Parser parser(source, tokens);
    auto ast = parser.parse();
    ASSERT_EQ(ast->children.size(), 1);
    auto element_node = dynamic_cast<ElementNode*>(ast->children[0].get());
    ASSERT_NE(element_node, nullptr);
    EXPECT_EQ(element_node->tag_name, "div");
    EXPECT_EQ(element_node->attributes.size(), 2);
    EXPECT_EQ(element_node->attributes["id"], "main");
    EXPECT_EQ(element_node->attributes["class"], "container");
}

TEST(ParserTest, ElementWithInlineStyle) {
    std::string source = "div { style { color: red; font-size: 16px; } }";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    Parser parser(source, tokens);
    auto ast = parser.parse();
    ASSERT_EQ(ast->children.size(), 1);
    auto element_node = dynamic_cast<ElementNode*>(ast->children[0].get());
    ASSERT_NE(element_node, nullptr);
    EXPECT_EQ(element_node->tag_name, "div");
    ASSERT_EQ(element_node->children.size(), 1);
    auto style_node = dynamic_cast<StyleNode*>(element_node->children[0].get());
    ASSERT_NE(style_node, nullptr);
    ASSERT_EQ(style_node->children.size(), 2);
    auto prop1 = dynamic_cast<StylePropertyNode*>(style_node->children[0].get());
    ASSERT_NE(prop1, nullptr);
    EXPECT_EQ(prop1->key, "color");
    EXPECT_EQ(prop1->value, "red");
    auto prop2 = dynamic_cast<StylePropertyNode*>(style_node->children[1].get());
    ASSERT_NE(prop2, nullptr);
    EXPECT_EQ(prop2->key, "font-size");
    EXPECT_EQ(prop2->value, "16px");
}

TEST(ParserTest, ElementWithGlobalStyle) {
    std::string source = "div { style { .my-class { color: red; } } }";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    Parser parser(source, tokens);
    auto ast = parser.parse();
    ASSERT_EQ(ast->children.size(), 1);
    auto element_node = dynamic_cast<ElementNode*>(ast->children[0].get());
    ASSERT_NE(element_node, nullptr);
    EXPECT_EQ(element_node->tag_name, "div");
    ASSERT_EQ(element_node->children.size(), 1);
    auto style_node = dynamic_cast<StyleNode*>(element_node->children[0].get());
    ASSERT_NE(style_node, nullptr);
    ASSERT_EQ(style_node->children.size(), 1);
    auto rule_node = dynamic_cast<StyleRuleNode*>(style_node->children[0].get());
    ASSERT_NE(rule_node, nullptr);
    EXPECT_EQ(rule_node->selector, "my-class");
    ASSERT_EQ(rule_node->children.size(), 1);
    auto prop_node = dynamic_cast<StylePropertyNode*>(rule_node->children[0].get());
    ASSERT_NE(prop_node, nullptr);
    EXPECT_EQ(prop_node->key, "color");
    EXPECT_EQ(prop_node->value, "red");
}

TEST(ParserTest, ElementWithScript) {
    std::string source = "div { script { let x = 1; } }";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    Parser parser(source, tokens);
    auto ast = parser.parse();
    ASSERT_EQ(ast->children.size(), 1);
    auto element_node = dynamic_cast<ElementNode*>(ast->children[0].get());
    ASSERT_NE(element_node, nullptr);
    EXPECT_EQ(element_node->tag_name, "div");
    ASSERT_EQ(element_node->children.size(), 1);
    auto script_node = dynamic_cast<ScriptNode*>(element_node->children[0].get());
    ASSERT_NE(script_node, nullptr);
    EXPECT_EQ(script_node->content, "let x = 1;");
}

TEST(ParserTest, ElementTemplate) {
    std::string source = "[Template] @Element MyElement { div { text: \"hello\"; } }";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    Parser parser(source, tokens);
    auto ast = parser.parse();
    ASSERT_EQ(ast->children.size(), 1);
    auto template_node = dynamic_cast<TemplateNode*>(ast->children[0].get());
    ASSERT_NE(template_node, nullptr);
    EXPECT_EQ(template_node->name, "MyElement");
    EXPECT_EQ(template_node->type, TemplateType::Element);
    ASSERT_EQ(template_node->children.size(), 1);
    auto element_node = dynamic_cast<ElementNode*>(template_node->children[0].get());
    ASSERT_NE(element_node, nullptr);
    EXPECT_EQ(element_node->tag_name, "div");
}

TEST(ParserTest, StyleTemplate) {
    std::string source = "[Template] @Style MyStyle { color: red; }";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    Parser parser(source, tokens);
    auto ast = parser.parse();
    ASSERT_EQ(ast->children.size(), 1);
    auto template_node = dynamic_cast<TemplateNode*>(ast->children[0].get());
    ASSERT_NE(template_node, nullptr);
    EXPECT_EQ(template_node->name, "MyStyle");
    EXPECT_EQ(template_node->type, TemplateType::Style);
    ASSERT_EQ(template_node->children.size(), 1);
    auto prop_node = dynamic_cast<StylePropertyNode*>(template_node->children[0].get());
    ASSERT_NE(prop_node, nullptr);
    EXPECT_EQ(prop_node->key, "color");
    EXPECT_EQ(prop_node->value, "red");
}

TEST(ParserTest, VarTemplate) {
    std::string source = "[Template] @Var MyVars { color: \"red\"; }";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    Parser parser(source, tokens);
    auto ast = parser.parse();
    ASSERT_EQ(ast->children.size(), 1);
    auto template_node = dynamic_cast<TemplateNode*>(ast->children[0].get());
    ASSERT_NE(template_node, nullptr);
    EXPECT_EQ(template_node->name, "MyVars");
    EXPECT_EQ(template_node->type, TemplateType::Var);
    EXPECT_EQ(template_node->variables.size(), 1);
    EXPECT_EQ(template_node->variables["color"], "red");
}
