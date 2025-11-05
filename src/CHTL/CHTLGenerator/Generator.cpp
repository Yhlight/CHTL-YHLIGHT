#include "Generator.h"
#include "ElementNode.h"
#include "TextNode.h"
#include "StyleNode.h"
#include "StylePropertyNode.h"
#include "StyleRuleNode.h"
#include "ScriptNode.h"

Generator::Generator(const BaseNode& root) : root(root) {}

std::string Generator::generate(bool full_document) {
    visit(&root);

    if (full_document) {
        std::stringstream final_output;
        final_output << "<html><head><style>" << css_output.str() << "</style></head><body>";
        final_output << html_output.str();
        final_output << "<script>" << js_output.str() << "</script>";
        final_output << "</body></html>";
        return final_output.str();
    } else {
        return html_output.str();
    }
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
        case NodeType::Script:
            visit(static_cast<const ScriptNode*>(node));
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
            for (const auto& style_content : style_node->children) {
                if (style_content->getStyleContentType() == StyleContentType::Property) {
                    auto prop_node = static_cast<const StylePropertyNode*>(style_content.get());
                    style_attr << prop_node->key << ":" << prop_node->value << ";";
                } else if (style_content->getStyleContentType() == StyleContentType::Rule) {
                    auto rule_node = static_cast<const StyleRuleNode*>(style_content.get());
                    css_output << "." << rule_node->selector << "{";
                    for (const auto& prop : rule_node->children) {
                        auto prop_node = static_cast<const StylePropertyNode*>(prop.get());
                        css_output << prop_node->key << ":" << prop_node->value << ";";
                    }
                    css_output << "}";
                    html_output << " class=\"" << rule_node->selector << "\"";
                }
            }
        }
    }

    if (style_attr.str().length() > 0) {
        html_output << " style=\"" << style_attr.str() << "\"";
    }

    html_output << ">";
    for (const auto& child : node->children) {
        if (child->getType() != NodeType::Style) {
            visit(child.get());
        }
    }
    html_output << "</" << node->tag_name << ">";
}

void Generator::visit(const TextNode* node) {
    html_output << node->text;
}

void Generator::visit(const ScriptNode* node) {
    js_output << node->content;
}
