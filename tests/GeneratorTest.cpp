#include "gtest/gtest.h"
#include "Generator.h"
#include "ElementNode.h"
#include "TextNode.h"
#include "StyleNode.h"

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
    EXPECT_NE(html.find("id=\"main\""), std::string::npos);
    EXPECT_NE(html.find("class=\"container\""), std::string::npos);
}

TEST(GeneratorTest, ElementWithInlineStyle) {
    auto root = std::make_unique<ElementNode>("div");
    auto style_node = std::make_unique<StyleNode>();
    style_node->properties["color"] = "red";
    style_node->properties["font-size"] = "16px";
    root->children.push_back(std::move(style_node));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_NE(html.find("style=\"color:red;font-size:16px;\""), std::string::npos);
}
