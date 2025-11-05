#include "gtest/gtest.h"
#include "Generator.h"
#include "ElementNode.h"
#include "TextNode.h"

TEST(GeneratorTest, SimpleElement) {
    auto root = std::make_unique<ElementNode>("div");
    root->children.push_back(std::make_unique<TextNode>("hello"));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<div>hello</div>");
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
