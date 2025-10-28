#include "Generator.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/StyleBlockNode.h"
#include "CHTL/CHTLNode/TemplateStyleDefinitionNode.h"

namespace CHTL {

Generator::Generator(std::shared_ptr<BaseNode> root, const std::map<std::string, std::shared_ptr<TemplateStyleDefinitionNode>>& styleTemplates)
    : m_root(root), m_styleTemplates(styleTemplates) {}

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

            if (elementNode->getStyleBlock()) {
                std::string styleContent;
                for (const auto& templateName : elementNode->getStyleBlock()->getUsedTemplates()) {
                    if (m_styleTemplates.count(templateName)) {
                        styleContent += m_styleTemplates.at(templateName)->getStyleBlock()->getRawContent();
                    }
                }
                styleContent += elementNode->getStyleBlock()->getRawContent();
                if (!styleContent.empty()) {
                    output += " style=\"" + styleContent + "\"";
                }
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
            // For now, we don't generate output for template definitions themselves
            break;
    }
}

} // namespace CHTL
