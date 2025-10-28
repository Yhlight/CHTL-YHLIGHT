#include "Generator.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/TextNode.h"

namespace CHTL {

Generator::Generator(std::shared_ptr<BaseNode> root) : m_root(root) {}

std::string Generator::generate() {
    std::string output;
    visit(m_root, output);
    return output;
}

void Generator::visit(std::shared_ptr<BaseNode> node, std::string& output) {
    if (!node) return;

    switch (node->getType()) {
        case NodeType::Element: {
            auto elementNode = std::static_pointer_cast<ElementNode>(node);
            output += "<" + elementNode->getTagName();
            for (const auto& attr : elementNode->getAttributes()) {
                output += " " + attr->getKey() + "=\"" + attr->getValue() + "\"";
            }
            output += ">";
            for (const auto& child : elementNode->getChildren()) {
                visit(child, output);
            }
            output += "</" + elementNode->getTagName() + ">";
            break;
        }
        case NodeType::Text: {
            auto textNode = std::static_pointer_cast<TextNode>(node);
            output += textNode->getText();
            break;
        }
        default:
            // Handle other node types in the future
            break;
    }
}

} // namespace CHTL
