#include "Generator.h"
#include <sstream>

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

        ss << ">";

        for (const auto& child : elementNode->children) {
            ss << generate(*child);
        }

        ss << "</" << elementNode->tagName << ">";
        return ss.str();
    }

    return "";
}
