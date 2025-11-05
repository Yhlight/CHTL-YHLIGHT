#include "gtest/gtest.h"
#include "Generator.h"
#include "ProgramNode.h"
#include "ElementNode.h"
#include "TextNode.h"
#include "StyleNode.h"
#include "StylePropertyNode.h"
#include "StyleRuleNode.h"
#include "ScriptNode.h"
#include "TemplateNode.h"
#include "ElementDirectiveNode.h"

TEST(GeneratorTest, SimpleElement) {
    auto root = std::make_unique<ProgramNode>();
    auto element = std::make_unique<ElementNode>("div");
    element->children.push_back(std::make_unique<TextNode>("hello"));
    root->children.push_back(std::move(element));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style></style></head><body><div>hello</div><script></script></body></html>");
}

TEST(GeneratorTest, SimpleElementRaw) {
    auto root = std::make_unique<ProgramNode>();
    auto element = std::make_unique<ElementNode>("div");
    element->children.push_back(std::make_unique<TextNode>("hello"));
    root->children.push_back(std::move(element));
    Generator generator(*root);
    auto html = generator.generate(false);
    EXPECT_EQ(html, "<div>hello</div>");
}

TEST(GeneratorTest, ElementWithAttributes) {
    auto root = std::make_unique<ProgramNode>();
    auto element = std::make_unique<ElementNode>("div");
    element->attributes["id"] = "main";
    element->attributes["class"] = "container";
    root->children.push_back(std::move(element));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style></style></head><body><div class=\"container\" id=\"main\"></div><script></script></body></html>");
}

TEST(GeneratorTest, ElementWithInlineStyle) {
    auto root = std::make_unique<ProgramNode>();
    auto element = std::make_unique<ElementNode>("div");
    auto style_node = std::make_unique<StyleNode>();
    style_node->children.push_back(std::make_unique<StylePropertyNode>("color", "red"));
    style_node->children.push_back(std::make_unique<StylePropertyNode>("font-size", "16px"));
    element->children.push_back(std::move(style_node));
    root->children.push_back(std::move(element));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style></style></head><body><div style=\"color:red;font-size:16px;\"></div><script></script></body></html>");
}

TEST(GeneratorTest, ElementWithGlobalStyle) {
    auto root = std::make_unique<ProgramNode>();
    auto element = std::make_unique<ElementNode>("div");
    auto style_node = std::make_unique<StyleNode>();
    auto rule_node = std::make_unique<StyleRuleNode>("my-class");
    rule_node->children.push_back(std::make_unique<StylePropertyNode>("color", "red"));
    style_node->children.push_back(std::move(rule_node));
    element->children.push_back(std::move(style_node));
    root->children.push_back(std::move(element));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style>.my-class{color:red;}</style></head><body><div class=\"my-class\"></div><script></script></body></html>");
}

TEST(GeneratorTest, ElementWithScript) {
    auto root = std::make_unique<ProgramNode>();
    auto element = std::make_unique<ElementNode>("div");
    element->children.push_back(std::make_unique<ScriptNode>("let x = 1;"));
    root->children.push_back(std::move(element));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style></style></head><body><div></div><script>let x = 1;</script></body></html>");
}

TEST(GeneratorTest, ElementTemplate) {
    auto root = std::make_unique<ProgramNode>();
    auto template_node = std::make_unique<TemplateNode>("MyElement", TemplateType::Element);
    auto template_element = std::make_unique<ElementNode>("div");
    template_element->children.push_back(std::make_unique<TextNode>("hello"));
    template_node->children.push_back(std::move(template_element));
    root->children.push_back(std::move(template_node));
    auto element_directive = std::make_unique<ElementDirectiveNode>("MyElement");
    root->children.push_back(std::move(element_directive));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style></style></head><body><div>hello</div><script></script></body></html>");
}
