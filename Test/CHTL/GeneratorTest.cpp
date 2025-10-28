#include <gtest/gtest.h>
#include "CHTL/CHTLGenerator/Generator.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/AttributeNode.h"

TEST(GeneratorTest, GenerateHTMLWithAttribute) {
    // Manually construct an AST: div { id: "box" }
    auto attribute = std::make_shared<CHTL::AttributeNode>("id", "box");
    auto divNode = std::make_shared<CHTL::ElementNode>("div");
    divNode->addAttribute(attribute);

    CHTL::Generator generator(divNode);
    std::string result = generator.generate();

    std::string expected = "<div id=\"box\"></div>";
    EXPECT_EQ(result, expected);
}

TEST(GeneratorTest, GenerateSimpleHTML) {
    // Manually construct an AST: div { span { text { "hello" } } }
    auto textNode = std::make_shared<CHTL::TextNode>("hello");
    auto spanNode = std::make_shared<CHTL::ElementNode>("span");
    spanNode->addChild(textNode);
    auto divNode = std::make_shared<CHTL::ElementNode>("div");
    divNode->addChild(spanNode);

    CHTL::Generator generator(divNode);
    std::string result = generator.generate();

    std::string expected = "<div><span>hello</span></div>";
    EXPECT_EQ(result, expected);
}
