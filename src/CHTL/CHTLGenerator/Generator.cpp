#include "Generator.h"
#include "ElementNode.h"
#include "TextNode.h"

Generator::Generator(const BaseNode& root) : root(root) {}

std::string Generator::generate() {
    visit(&root);
    return output.str();
}

void Generator::visit(const BaseNode* node) {
    switch (node->getType()) {
        case NodeType::Element:
            visit(static_cast<const ElementNode*>(node));
            break;
        case NodeType::Text:
            visit(static_cast<const TextNode*>(node));
            break;
    }
}

void Generator::visit(const ElementNode* node) {
    output << "<" << node->tag_name;
    for (const auto& attr : node->attributes) {
        output << " " << attr.first << "=\"" << attr.second << "\"";
    }
    output << ">";
    for (const auto& child : node->children) {
        visit(child.get());
    }
    output << "</" << node->tag_name << ">";
}

void Generator::visit(const TextNode* node) {
    output << node->text;
}
