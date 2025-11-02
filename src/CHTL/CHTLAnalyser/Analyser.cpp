#include "Analyser.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/NamespaceNode.h"
#include "CHTL/CHTLNode/TemplateNode.h"
#include "CHTL/CHTLNode/StyleNode.h"
#include "CHTL/CHTLNode/TemplateUsageNode.h"
#include "CHTL/CHTLNode/StylePropertyNode.h"
#include <iostream>

namespace CHTL {

Analyser::Analyser(ASTNode& root) : m_root(root) {}

void Analyser::analyse() {
    if (m_root.getType() == ASTNodeType::Program) {
        ProgramNode* programNode = static_cast<ProgramNode*>(&m_root);
        collect(programNode->children);
        resolve(&m_root);
    }
}

void Analyser::collect(std::vector<std::unique_ptr<ASTNode>>& nodes) {
    for (auto it = nodes.begin(); it != nodes.end(); ) {
        if ((*it)->getType() == ASTNodeType::Template || (*it)->getType() == ASTNodeType::CustomTemplate) {
            TemplateNode* templateNode = static_cast<TemplateNode*>((*it).get()); // Works for both
            if (templateNode->templateType == TemplateType::Style) {
                m_symbolTable.insertStyleTemplate(templateNode->name, templateNode);
                m_ownedTemplates.push_back(std::move(*it));
            }
            it = nodes.erase(it);
        } else {
            ++it;
        }
    }
}

void Analyser::resolve(ASTNode* node) {
    if (!node) return;

    switch (node->getType()) {
        case ASTNodeType::Program: {
            ProgramNode* programNode = static_cast<ProgramNode*>(node);
            for (auto& child : programNode->children) {
                resolve(child.get());
            }
            break;
        }
        case ASTNodeType::Element: {
            ElementNode* elementNode = static_cast<ElementNode*>(node);
            for (auto& child : elementNode->children) {
                resolve(child.get());
            }
            break;
        }
        case ASTNodeType::StyleBlock: {
            StyleNode* styleNode = static_cast<StyleNode*>(node);
            resolve(styleNode);
            break;
        }
        case ASTNodeType::Namespace: {
            NamespaceNode* namespaceNode = static_cast<NamespaceNode*>(node);
            for (auto& child : namespaceNode->children) {
                resolve(child.get());
            }
            break;
        }
        default:
            break;
    }
}

void Analyser::resolve(StyleNode* node) {
    std::vector<std::unique_ptr<ASTNode>> new_properties;
    for (auto& prop : node->properties) {
        if (prop->getType() == ASTNodeType::TemplateUsage) {
            TemplateUsageNode* usageNode = static_cast<TemplateUsageNode*>(prop.get());
            TemplateNode* templateNode = m_symbolTable.lookupStyleTemplate(usageNode->name);
            if (templateNode) {
                StyleNode* templateStyleNode = static_cast<StyleNode*>(templateNode->children[0].get());

                std::unordered_map<std::string, std::string> specialization_props;
                if (usageNode->specialization) {
                    for (const auto& spec_prop : usageNode->specialization->properties) {
                        if (spec_prop->getType() == ASTNodeType::StyleProperty) {
                            StylePropertyNode* specPropNode = static_cast<StylePropertyNode*>(spec_prop.get());
                            if (specPropNode->value.has_value()) {
                                specialization_props[specPropNode->name] = specPropNode->value.value();
                            }
                        }
                    }
                }

                for (const auto& template_prop : templateStyleNode->properties) {
                    if (template_prop->getType() == ASTNodeType::StyleProperty) {
                        StylePropertyNode* propNode = static_cast<StylePropertyNode*>(template_prop.get());
                        auto newProp = std::make_unique<StylePropertyNode>();
                        newProp->name = propNode->name;

                        if (specialization_props.count(propNode->name)) {
                            newProp->value = specialization_props[propNode->name];
                        } else {
                            newProp->value = propNode->value;
                        }
                        new_properties.push_back(std::move(newProp));
                    }
                }
            }
        } else {
            new_properties.push_back(std::move(prop));
        }
    }
    node->properties = std::move(new_properties);
}


} // namespace CHTL
