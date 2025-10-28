#include "../CHTLGenerator/Generator.h"
#include "../CHTLNode/AST.h"
#include <iostream>
#include <cassert>

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

int main() {
    testHtmlGenerationForTextNode();
    testHtmlGenerationForElementNode();
    std::cout << "All generator tests passed!" << std::endl;
    return 0;
}
