#include "gtest/gtest.h"
#include "Generator.h"
#include "ElementNode.h"
#include "TextNode.h"
#include "StyleNode.h"
#include "StylePropertyNode.h"
#include "StyleRuleNode.h"

TEST(GeneratorTest, SimpleElement) {
    auto root = std::make_unique<ElementNode>("div");
    root->children.push_back(std::make_unique<TextNode>("hello"));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style></style></head><body><div>hello</div></body></html>");
}

TEST(GeneratorTest, ElementWithAttributes) {
    auto root = std::make_unique<ElementNode>("div");
    root->attributes["id"] = "main";
    root->attributes["class"] = "container";
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style></style></head><body><div class=\"container\" id=\"main\"></div></body></html>");
}

TEST(GeneratorTest, ElementWithInlineStyle) {
    auto root = std::make_unique<ElementNode>("div");
    auto style_node = std::make_unique<StyleNode>();
    style_node->children.push_back(std::make_unique<StylePropertyNode>("color", "red"));
    style_node->children.push_back(std::make_unique<StylePropertyNode>("font-size", "16px"));
    root->children.push_back(std::move(style_node));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style></style></head><body><div style=\"color:red;font-size:16px;\"></div></body></html>");
}

TEST(GeneratorTest, ElementWithGlobalStyle) {
    auto root = std::make_unique<ElementNode>("div");
    auto style_node = std::make_unique<StyleNode>();
    auto rule_node = std::make_unique<StyleRuleNode>("my-class");
    rule_node->children.push_back(std::make_unique<StylePropertyNode>("color", "red"));
    style_node->children.push_back(std::move(rule_node));
    root->children.push_back(std::move(style_node));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style>.my-class{color:red;}</style></head><body><div class=\"my-class\"></div></body></html>");
}
