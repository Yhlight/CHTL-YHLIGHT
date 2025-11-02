#include "Analyser.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/NamespaceNode.h"
#include "CHTL/CHTLNode/TemplateNode.h"
#include "CHTL/CHTLNode/StyleNode.h"
#include "CHTL/CHTLNode/TemplateUsageNode.h"
#include "CHTL/CHTLNode/StylePropertyNode.h"
#include "CHTL/CHTLNode/ImportNode.h"
#include "CHTL/CHTLNode/ConfigurationNode.h"
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include <iostream>
#include <algorithm>

namespace CHTL {

Analyser::Analyser(ASTNode& root, const std::string& filePath, std::vector<std::string>& importStack)
    : m_root(root), m_filePath(filePath), m_importStack(importStack) {}

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
        case ASTNodeType::Import: {
            ImportNode* importNode = static_cast<ImportNode*>(node);
            std::string path = importNode->path;
            path.erase(std::remove(path.begin(), path.end(), '"'), path.end());

            std::string content = m_importer.importFile(m_filePath, path);

            std::string canonical_path = std::filesystem::weakly_canonical(std::filesystem::path(m_filePath).parent_path() / path).string();
            if (std::find(m_importStack.begin(), m_importStack.end(), canonical_path) != m_importStack.end()) {
                throw std::runtime_error("Circular import detected: " + canonical_path);
            }
            m_importStack.push_back(canonical_path);

            Lexer lexer(content);
            auto tokens = lexer.scanTokens();
            Parser parser(tokens, content);
            auto ast = parser.parse();

            Analyser subAnalyser(*ast, canonical_path, m_importStack);
            subAnalyser.analyse();

            for (auto const& [key, val] : subAnalyser.m_symbolTable.getStyleTemplates()) {
                m_symbolTable.insertStyleTemplate(key, val);
            }

            auto owned = subAnalyser.getOwnedTemplates();
            m_ownedTemplates.insert(m_ownedTemplates.end(), std::make_move_iterator(owned.begin()), std::make_move_iterator(owned.end()));

            m_importStack.pop_back();

            break;
        }
        case ASTNodeType::Configuration: {
            ConfigurationNode* configNode = static_cast<ConfigurationNode*>(node);
            resolve(configNode);
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

void Analyser::resolve(ConfigurationNode* node) {
    for (const auto& [key, value] : node->settings) {
        m_config[key] = value;
    }
    for (const auto& [key, value] : node->nameGroup) {
        m_config[key] = value;
    }
}


} // namespace CHTL
