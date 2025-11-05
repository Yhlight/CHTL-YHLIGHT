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
