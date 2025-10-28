#include "../CHTLGenerator/Generator.h"
#include "../CHTLNode/AST.h"
#include <iostream>
#include <cassert>
#include <string>

void testHtmlGenerationForTextNode() {
    TextNode node("Hello, CHTL!");
    Generator generator;
    std::string html = generator.generate(node);
    assert(html == "Hello, CHTL!");
}

void testHtmlGenerationForElementNode() {
    ElementNode node("div");
    node.attributes["id"] = "box";
    node.children.push_back(std::make_unique<TextNode>("Hello, World!"));
    Generator generator;
    std::string html = generator.generate(node);
    assert(html == "<div id=\"box\">Hello, World!</div>");
}

void testHtmlGenerationForInlineStyle() {
    ElementNode node("div");
    auto styleNode = std::make_unique<StyleNode>();
    styleNode->properties["width"] = {"100px"};
    styleNode->properties["height"] = {"200px"};
    node.children.push_back(std::move(styleNode));
    Generator generator;
    std::string html = generator.generate(node);

    std::string expected1 = "<div style=\"width:100px;height:200px;\"></div>";
    std::string expected2 = "<div style=\"height:200px;width:100px;\"></div>";

    assert(html == expected1 || html == expected2);
}

int main() {
    testHtmlGenerationForTextNode();
    testHtmlGenerationForElementNode();
    testHtmlGenerationForInlineStyle();
    std::cout << "All generator tests passed!" << std::endl;
    return 0;
}
