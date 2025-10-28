#include "Generator.h"

void Generator::generateStyle(std::stringstream& ss, const ElementNode& node) {
    bool hasStyle = false;
    for (const auto& child : node.children) {
        if (dynamic_cast<const StyleNode*>(child.get())) {
            hasStyle = true;
            break;
        }
    }
    if (!hasStyle) return;

    ss << " style=\"";
    for (const auto& child : node.children) {
        if (const auto* styleNode = dynamic_cast<const StyleNode*>(child.get())) {
            for (const auto& prop : styleNode->properties) {
                ss << prop.first << ":" << prop.second.value << ";";
            }
        }
    }
    ss << "\"";
}

std::string Generator::generate(const ASTNode& node) {
    if (const auto* textNode = dynamic_cast<const TextNode*>(&node)) {
        return textNode->text;
    }

    if (const auto* elementNode = dynamic_cast<const ElementNode*>(&node)) {
        std::stringstream ss;
        ss << "<" << elementNode->tagName;

        for (const auto& attr : elementNode->attributes) {
            ss << " " << attr.first << "=\"" << attr.second << "\"";
        }

        generateStyle(ss, *elementNode);

        ss << ">";

        for (const auto& child : elementNode->children) {
            if (!dynamic_cast<const StyleNode*>(child.get())) {
                ss << generate(*child);
            }
        }

        ss << "</" << elementNode->tagName << ">";
        return ss.str();
    }

    return "";
}
