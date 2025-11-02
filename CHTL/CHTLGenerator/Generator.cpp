#include "Generator.h"
#include "CHTLNode/ElementNode.h"
#include "CHTLNode/TextNode.h"
#include <iostream>

Generator::Generator(AstNodeList ast) : ast(std::move(ast)) {}

std::string Generator::generate() {
    for (const auto& node : ast) {
        visit(node.get());
    }
    return html_output;
}

void Generator::visit(BaseNode* node) {
    if (auto elementNode = dynamic_cast<ElementNode*>(node)) {
        visitElement(elementNode);
    } else if (auto textNode = dynamic_cast<TextNode*>(node)) {
        visitText(textNode);
    }
}

void Generator::visitElement(ElementNode* node) {
    html_output += "<" + node->tag_name;
    for (const auto& attr : node->attributes) {
        html_output += " " + attr.first + "=\"" + attr.second + "\"";
    }
    html_output += ">";

    for (const auto& child : node->children) {
        visit(child.get());
    }

    html_output += "</" + node->tag_name + ">";
}

void Generator::visitText(TextNode* node) {
    html_output += node->text;
}
