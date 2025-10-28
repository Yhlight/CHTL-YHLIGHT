#include <gtest/gtest.h>
#include "CHTL/CHTLGenerator/Generator.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/AttributeNode.h"
#include "CHTL/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"

TEST(GeneratorTest, EndToEndStyleTemplate) {
    std::string source = R"(
        [Template] @Style MyStyles { color: red; }
        div {
            style {
                @Style MyStyles;
                font-size: 16px;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    std::shared_ptr<CHTL::BaseNode> root = parser.parse();

    CHTL::Generator generator(root, parser.getStyleTemplates());
    std::string result = generator.generate();

    std::string expected = "<div style=\"color:red;font-size:16px;\"></div>";
    EXPECT_EQ(result, expected);
}


TEST(GeneratorTest, GenerateHTMLWithAttribute) {
    // Manually construct an AST: div { id: "box" }
    auto attribute = std::make_shared<CHTL::AttributeNode>("id", "box");
    auto divNode = std::make_shared<CHTL::ElementNode>("div");
    divNode->addAttribute(attribute);

    std::map<std::string, std::shared_ptr<CHTL::TemplateStyleDefinitionNode>> emptyMap;
    CHTL::Generator generator(divNode, emptyMap);
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

    std::map<std::string, std::shared_ptr<CHTL::TemplateStyleDefinitionNode>> emptyMap;
    CHTL::Generator generator(divNode, emptyMap);
    std::string result = generator.generate();

    std::string expected = "<div><span>hello</span></div>";
    EXPECT_EQ(result, expected);
}
