#include "Generator.h"

namespace CHTL {

std::string Generator::generate(const ProgramNode& program) {
    visit(&program);

    std::string css = css_output.str();
    if (!css.empty()) {
        return "<style>" + css + "</style>" + html_output.str();
    }

    return html_output.str();
}

void Generator::visit(const ProgramNode* node) {
    for (const auto& statement : node->statements) {
        switch (statement->getType()) {
            case NodeType::Element:
                visit(static_cast<ElementNode*>(statement.get()));
                break;
            default:
                break;
        }
    }
}

void Generator::visit(const ElementNode* node) {
    // Process style nodes first to populate attributes
    for (const auto& child : node->children) {
        if (child->getType() == NodeType::Style) {
            visit(static_cast<StyleNode*>(child.get()), const_cast<ElementNode*>(node));
        }
    }

    html_output << "<" << node->tagName;

    for (const auto& attr : node->attributes) {
        html_output << " " << attr.first << "=\"" << attr.second << "\"";
    }

    html_output << ">";

    for (const auto& child : node->children) {
        switch (child->getType()) {
            case NodeType::Element:
                visit(static_cast<ElementNode*>(child.get()));
                break;
            case NodeType::Text:
                visit(static_cast<TextNode*>(child.get()));
                break;
            case NodeType::Style:
                // Already processed
                break;
            default:
                break;
        }
    }

    html_output << "</" << node->tagName << ">";
}

void Generator::visit(const TextNode* node) {
    html_output << node->content;
}

void Generator::visit(const StyleNode* node, ElementNode* parent) {
    std::stringstream styleStream;
    for (const auto& child : node->children) {
        if (child->getType() == NodeType::StyleProperty) {
            visit(static_cast<StylePropertyNode*>(child.get()), styleStream);
        } else if (child->getType() == NodeType::StyleRule) {
            auto ruleNode = static_cast<StyleRuleNode*>(child.get());
            visit(ruleNode);
            // Assuming the selector is a class, add it to the parent
            if (ruleNode->selector[0] == '.') {
                 parent->attributes["class"] = ruleNode->selector.substr(1);
            }
        }
    }
    std::string styleString = styleStream.str();
    if (!styleString.empty()) {
        parent->attributes["style"] = styleString;
    }
}

void Generator::visit(const StylePropertyNode* node, std::stringstream& styleStream) {
    styleStream << node->key << ":" << node->value << ";";
}

void Generator::visit(const StyleRuleNode* node) {
    css_output << node->selector << "{";
    for (const auto& prop : node->properties) {
        css_output << prop->key << ":" << prop->value << ";";
    }
    css_output << "}";
}

} // namespace CHTL
