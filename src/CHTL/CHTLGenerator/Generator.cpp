#include "Generator.h"
#include "ElementNode.h"
#include "TextNode.h"
#include "StyleNode.h"

Generator::Generator(const BaseNode& root) : root(root) {}

std::string Generator::generate() {
    visit(&root);

    std::stringstream final_output;
    final_output << "<html><head><style>" << css_output.str() << "</style></head><body>";
    final_output << html_output.str();
    final_output << "</body></html>";
    return final_output.str();
}

void Generator::visit(const BaseNode* node) {
    switch (node->getType()) {
        case NodeType::Element:
            visit(static_cast<const ElementNode*>(node));
            break;
        case NodeType::Text:
            visit(static_cast<const TextNode*>(node));
            break;
        case NodeType::Style:
            // Style nodes are handled by the parent element
            break;
    }
}

void Generator::visit(const ElementNode* node) {
    html_output << "<" << node->tag_name;
    for (const auto& attr : node->attributes) {
        html_output << " " << attr.first << "=\"" << attr.second << "\"";
    }

    std::stringstream style_attr;
    for (const auto& child : node->children) {
        if (child->getType() == NodeType::Style) {
            auto style_node = static_cast<const StyleNode*>(child.get());
            for (const auto& prop : style_node->properties) {
                style_attr << prop.first << ":" << prop.second << ";";
            }
        }
    }

    if (style_attr.str().length() > 0) {
        html_output << " style=\"" << style_attr.str() << "\"";
    }

    html_output << ">";
    for (const auto& child : node->children) {
        visit(child.get());
    }
    html_output << "</" << node->tag_name << ">";
}

void Generator::visit(const TextNode* node) {
    html_output << node->text;
}
