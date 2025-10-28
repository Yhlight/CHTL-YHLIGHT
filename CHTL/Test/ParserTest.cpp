#include "../CHTLParser/Parser.h"
#include "../CHTLLexer/Lexer.h"
#include <iostream>
#include <cassert>

void testParsingOfTextNode() {
    std::string source = "text { \"Hello, CHTL!\" }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    std::unique_ptr<ASTNode> node = parser.parse();

    assert(node != nullptr);
    TextNode* textNode = dynamic_cast<TextNode*>(node.get());
    assert(textNode != nullptr);
    assert(textNode->text == "Hello, CHTL!");
}

void testParsingOfElementNode() {
    std::string source = "div {}";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    std::unique_ptr<ASTNode> node = parser.parse();

    assert(node != nullptr);
    ElementNode* elementNode = dynamic_cast<ElementNode*>(node.get());
    assert(elementNode != nullptr);
    assert(elementNode->tagName == "div");
}

void testParsingOfElementWithAttributes() {
    std::string source = "div { id: \"box\"; class = \"container\"; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    std::unique_ptr<ASTNode> node = parser.parse();

    assert(node != nullptr);
    ElementNode* elementNode = dynamic_cast<ElementNode*>(node.get());
    assert(elementNode != nullptr);
    assert(elementNode->tagName == "div");
    assert(elementNode->attributes.size() == 2);
    assert(elementNode->attributes.at("id") == "box");
    assert(elementNode->attributes.at("class") == "container");
}

void testParsingOfNestedElements() {
    std::string source = "body { div {} }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    std::unique_ptr<ASTNode> node = parser.parse();

    assert(node != nullptr);
    ElementNode* bodyNode = dynamic_cast<ElementNode*>(node.get());
    assert(bodyNode != nullptr);
    assert(bodyNode->tagName == "body");
    assert(bodyNode->children.size() == 1);

    ElementNode* divNode = dynamic_cast<ElementNode*>(bodyNode->children[0].get());
    assert(divNode != nullptr);
    assert(divNode->tagName == "div");
}

void testParsingOfStyleBlock() {
    std::string source = "div { style { width: 100px; } }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    std::unique_ptr<ASTNode> node = parser.parse();

    assert(node != nullptr);
    ElementNode* elementNode = dynamic_cast<ElementNode*>(node.get());
    assert(elementNode != nullptr);
    assert(elementNode->tagName == "div");
    assert(elementNode->children.size() == 1);

    StyleNode* styleNode = dynamic_cast<StyleNode*>(elementNode->children[0].get());
    assert(styleNode != nullptr);
    assert(styleNode->properties.size() == 1);
    assert(styleNode->properties.at("width").value == "100px");
}

int main() {
    testParsingOfTextNode();
    testParsingOfElementNode();
    testParsingOfElementWithAttributes();
    testParsingOfNestedElements();
    testParsingOfStyleBlock();
    std::cout << "All parser tests passed!" << std::endl;
    return 0;
}
