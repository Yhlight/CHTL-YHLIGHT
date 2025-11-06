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
    std::string firstClassSelector;
    std::string firstIdSelector;

    // First pass: Find the first class and id selectors to determine the context
    for (const auto& child : node->children) {
        if (child->getType() == NodeType::StyleRule) {
            auto ruleNode = static_cast<const StyleRuleNode*>(child.get());
            const std::string& selector = ruleNode->selector;
            if (!selector.empty()) {
                if (selector[0] == '.' && firstClassSelector.empty()) {
                    firstClassSelector = selector;
                } else if (selector[0] == '#' && firstIdSelector.empty()) {
                    firstIdSelector = selector;
                }
            }
        }
    }

    // Determine the primary selector for '&' replacement (class has priority)
    std::string primarySelectorForContext = !firstClassSelector.empty() ? firstClassSelector : firstIdSelector;

    // Automatically add class or id attributes
    if (!firstClassSelector.empty()) {
        if (parent->attributes.count("class")) {
            parent->attributes["class"] += " " + firstClassSelector.substr(1);
        } else {
            parent->attributes["class"] = firstClassSelector.substr(1);
        }
    }
    if (!firstIdSelector.empty()) {
        parent->attributes["id"] = firstIdSelector.substr(1);
    }

    // Second pass: Generate CSS for all rules
    for (const auto& child : node->children) {
        if (child->getType() == NodeType::StyleProperty) {
            visit(static_cast<StylePropertyNode*>(child.get()), styleStream);
        } else if (child->getType() == NodeType::StyleRule) {
            auto ruleNode = static_cast<const StyleRuleNode*>(child.get());
            std::string resolvedSelector = ruleNode->selector;

            // Resolve '&' selectors
            if (!resolvedSelector.empty() && resolvedSelector[0] == '&') {
                if (!primarySelectorForContext.empty()) {
                    resolvedSelector.replace(0, 1, primarySelectorForContext);
                } else {
                    // If no primary selector, just remove the '&'
                    resolvedSelector.erase(0, 1);
                }
            }

            // Generate the CSS for the rule
            css_output << resolvedSelector << "{";
            for (const auto& prop : ruleNode->properties) {
                css_output << prop->key << ":" << prop->value << ";";
            }
            css_output << "}";
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

} // namespace CHTL
