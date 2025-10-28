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
            resolveAndVisitElementTemplate(usageNode->getName(), output);
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

    for (const auto& templateName : styleBlock->getUsedTemplates()) {
        collectInheritedStyleProperties(templateName, properties);
    }

    // Then, add/override with properties from the current block
    for (const auto& prop : styleBlock->getProperties()) {
        properties[prop->getKey()] = resolveVariable(prop->getValue());
    }
}

void Generator::collectInheritedStyleProperties(const std::string& templateName, std::map<std::string, std::string>& properties) {
    if (!m_styleTemplates.count(templateName)) {
        return;
    }

    auto templateNode = m_styleTemplates.at(templateName);

    // First, handle the parent
    if (templateNode->getParentName()) {
        collectInheritedStyleProperties(templateNode->getParentName().value(), properties);
    }

    // Then, apply current template's styles
    collectStyleProperties(templateNode->getStyleBlock(), properties);
}

void Generator::resolveAndVisitElementTemplate(const std::string& templateName, std::string& output) {
    if (!m_elementTemplates.count(templateName)) {
        return;
    }

    auto templateNode = m_elementTemplates.at(templateName);

    // First, resolve the parent
    if (templateNode->getParentName()) {
        resolveAndVisitElementTemplate(templateNode->getParentName().value(), output);
    }

    // Then, visit children of the current template
    for (const auto& child : templateNode->getChildren()) {
        visit(child, output);
    }
}

std::optional<std::string> Generator::getVariableValue(const std::string& templateName, const std::string& key) {
    if (!m_varTemplates.count(templateName)) {
        return std::nullopt;
    }

    auto templateNode = m_varTemplates.at(templateName);
    const auto& variables = templateNode->getVariables();

    if (variables.count(key)) {
        return variables.at(key);
    }

    if (templateNode->getParentName()) {
        return getVariableValue(templateNode->getParentName().value(), key);
    }

    return std::nullopt;
}

std::string Generator::resolveVariable(const std::string& value) {
    std::regex varRegex("([a-zA-Z_][a-zA-Z0-9_]*)\\(([^)]+)\\)");
    std::smatch match;

    if (std::regex_match(value, match, varRegex)) {
        std::string varTemplateName = match[1];
        std::string varKey = match[2];
        if (auto varValue = getVariableValue(varTemplateName, varKey)) {
            return *varValue;
        }
    }

    return value;
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
