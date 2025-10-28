#include "Generator.h"
#include <regex>
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/StyleBlockNode.h"
#include "CHTL/CHTLNode/TemplateStyleDefinitionNode.h"
#include "CHTL/CHTLNode/TemplateElementUsageNode.h"
#include "CHTL/CHTLNode/ProgramNode.h"

namespace CHTL {

Generator::Generator(std::shared_ptr<BaseNode> root,
                     const std::map<std::string, std::shared_ptr<TemplateStyleDefinitionNode>>& styleTemplates,
                     const std::map<std::string, std::shared_ptr<TemplateElementDefinitionNode>>& elementTemplates,
                     const std::map<std::string, std::shared_ptr<TemplateVarDefinitionNode>>& varTemplates)
    : m_root(root), m_styleTemplates(styleTemplates), m_elementTemplates(elementTemplates), m_varTemplates(varTemplates) {}

std::string Generator::generate() {
    std::string output;
    visit(m_root, output);
    return output;
}

void Generator::visit(std::shared_ptr<BaseNode> node, std::string& output) {
    if (!node) return;

    switch (node->getType()) {
        case NodeType::Program: {
            auto programNode = std::static_pointer_cast<ProgramNode>(node);
            for (const auto& child : programNode->getChildren()) {
                visit(child, output);
            }
            break;
        }
        case NodeType::Element: {
            auto elementNode = std::static_pointer_cast<ElementNode>(node);
            output += "<" + elementNode->getTagName();
            for (const auto& attr : elementNode->getAttributes()) {
                output += " " + attr->getKey() + "=\"" + attr->getValue() + "\"";
            }

            if (elementNode->getStyleBlock()) {
                std::string styleContent = generateStyleContent(elementNode->getStyleBlock());
                if (!styleContent.empty()) {
                    // Trim trailing space before closing quote
                    if (styleContent.back() == ' ') {
                        styleContent.pop_back();
                    }
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
        case NodeType::TemplateElementUsage: {
            auto usageNode = std::static_pointer_cast<TemplateElementUsageNode>(node);
            if (m_elementTemplates.count(usageNode->getName())) {
                auto templateNode = m_elementTemplates.at(usageNode->getName());
                for (const auto& child : templateNode->getChildren()) {
                    visit(child, output);
                }
            }
            break;
        }
        default:
            // For now, we don't generate output for template definitions themselves
            break;
    }
}

void Generator::collectStyleProperties(std::shared_ptr<const StyleBlockNode> styleBlock, std::map<std::string, std::string>& properties) {
    if (!styleBlock) {
        return;
    }

    // First, recursively collect properties from inherited templates
    for (const auto& templateName : styleBlock->getUsedTemplates()) {
        if (m_styleTemplates.count(templateName)) {
            collectStyleProperties(m_styleTemplates.at(templateName)->getStyleBlock(), properties);
        }
    }

    // Then, add/override with properties from the current block
    for (const auto& prop : styleBlock->getProperties()) {
        std::string value = prop->getValue();

        std::regex varRegex("([a-zA-Z_][a-zA-Z0-9_]*)\\(([^)]+)\\)");
        std::smatch match;

        if (std::regex_match(value, match, varRegex)) {
            std::string varTemplateName = match[1];
            std::string varKey = match[2];

            if (m_varTemplates.count(varTemplateName)) {
                const auto& varTemplate = m_varTemplates.at(varTemplateName);
                if (varTemplate->getVariables().count(varKey)) {
                    value = varTemplate->getVariables().at(varKey);
                }
            }
        }
        properties[prop->getKey()] = value;
    }
}

std::string Generator::generateStyleContent(std::shared_ptr<const StyleBlockNode> styleBlock) {
    std::map<std::string, std::string> properties;
    collectStyleProperties(styleBlock, properties);

    std::string content;
    for (const auto& pair : properties) {
        content += pair.first + ":" + pair.second + ";";
    }
    return content;
}

} // namespace CHTL
