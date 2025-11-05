#include "Analyser.h"
#include "../CHTLNode/ProgramNode.h"
#include "../CHTLNode/TemplateNode.h"
#include "../CHTLNode/StyleNode.h"
#include "../CHTLNode/TemplateUsageNode.h"
#include "../CHTLNode/ElementNode.h"
#include "../CHTLNode/StylePropertyNode.h"
#include "../CHTLNode/ValueNode/ValueNode.h"
#include "../CHTLNode/ValueNode/VariableUsageNode.h"
#include "../CHTLNode/ValueNode/LiteralValueNode.h"
#include "../CHTLNode/ImportNode.h"
#include "../CHTLParser/Parser.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

namespace CHTL {

Analyser::Analyser(SymbolTable& symbolTable)
    : symbolTable_(symbolTable) {}

Analyser::~Analyser() = default;

void Analyser::analyse(ASTNode* root) {
    visit(root);
}

void Analyser::visit(ASTNode* node) {
    if (!node) return;
    switch (node->getType()) {
        case ASTNodeType::Program:
            visitProgramNode(node);
            break;
        case ASTNodeType::Style:
            visitStyleNode(node);
            break;
        case ASTNodeType::Element:
            visitElementNode(node);
            break;
        case ASTNodeType::StyleProperty:
            visitStylePropertyNode(node);
            break;
        case ASTNodeType::Import:
            visitImportNode(node);
            break;
        case ASTNodeType::Origin:
            visitOriginNode(node);
            break;
        default:
            for (const auto& child : node->children) {
                visit(child.get());
            }
            break;
    }
}

void Analyser::visitStylePropertyNode(ASTNode* node) {
    auto styleProp = dynamic_cast<StylePropertyNode*>(node);
    if (!styleProp) return;

    auto valueNode = dynamic_cast<ValueNode*>(styleProp->value.get());
    if (valueNode && valueNode->valueType == ValueNodeType::VariableUsage) {
        auto varUsage = dynamic_cast<VariableUsageNode*>(valueNode);
        auto* node = symbolTable_.lookup(varUsage->name);
        auto* templateNode = dynamic_cast<TemplateNode*>(node);
        if (templateNode && templateNode->templateType == "Var") {
            // A Var template should only have one property.
            if (!templateNode->children.empty()) {
                auto prop = dynamic_cast<StylePropertyNode*>(templateNode->children[0].get());
                if (prop) {
                    styleProp->value = std::unique_ptr<ValueNode>(static_cast<ValueNode*>(prop->value->clone().release()));
                }
            }
        }
    }
}

void Analyser::visitElementNode(ASTNode* node) {
    auto elementNode = dynamic_cast<ElementNode*>(node);
    if (!elementNode) return;

    std::vector<std::unique_ptr<ASTNode>> newChildren;
    for (auto& child : elementNode->children) {
        if (child->getType() == ASTNodeType::TemplateUsage) {
            auto usage = dynamic_cast<TemplateUsageNode*>(child.get());
            if (usage && usage->templateType == "Element") {
                auto* node = symbolTable_.lookup(usage->name);
                auto* templateNode = dynamic_cast<TemplateNode*>(node);
                if (templateNode) {
                    for (const auto& prop : templateNode->children) {
                        newChildren.push_back(prop->clone());
                    }
                }
            } else {
                newChildren.push_back(std::move(child));
            }
        } else {
            newChildren.push_back(std::move(child));
        }
    }
    elementNode->children = std::move(newChildren);

    for (const auto& child : elementNode->children) {
        visit(child.get());
    }
}

void Analyser::visitImportNode(ASTNode* node) {
    auto importNode = dynamic_cast<ImportNode*>(node);
    if (!importNode) return;

    if (importNode->importType == ImportType::Chtl) {
        try {
            std::string source = importer_.importFile(importNode->path);
            Parser parser(source);
            auto program = parser.parse();
            analyse(program.get());
        } catch (const std::runtime_error& e) {
            // It might be useful to log this error or handle it more gracefully.
            std::cerr << "Error importing file: " << e.what() << std::endl;
        }
    }
}

void Analyser::visitStyleNode(ASTNode* node) {
    auto styleNode = dynamic_cast<StyleNode*>(node);
    if (!styleNode) return;
    std::vector<std::unique_ptr<ASTNode>> newChildren;

    for (auto& child : styleNode->children) {
        if (child->getType() == ASTNodeType::TemplateUsage) {
            auto usage = dynamic_cast<TemplateUsageNode*>(child.get());
            if (usage) {
                auto* node = symbolTable_.lookup(usage->name);
                auto* templateNode = dynamic_cast<TemplateNode*>(node);
                if (templateNode) {
                    for (const auto& prop : templateNode->children) {
                        newChildren.push_back(prop->clone());
                    }
                }
            }
        } else {
            newChildren.push_back(std::move(child));
        }
    }
    styleNode->children = std::move(newChildren);

    for (auto& child : styleNode->children) {
        visit(child.get());
    }
}

void Analyser::visitProgramNode(ASTNode* node) {
    auto programNode = dynamic_cast<ProgramNode*>(node);
    if (!programNode) return;

    // First pass: collect templates
    for (auto it = programNode->children.begin(); it != programNode->children.end(); ) {
        if ((*it)->getType() == ASTNodeType::Template) {
            auto templateNode = dynamic_cast<TemplateNode*>((*it).get());
            if (templateNode) {
                symbolTable_.insert(templateNode->name, templateNode);
                ownedTemplates_.push_back(std::unique_ptr<TemplateNode>(static_cast<TemplateNode*>(it->release())));
                it = programNode->children.erase(it);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }

    // Visit remaining children
    for (const auto& child : programNode->children) {
        visit(child.get());
    }
}

void Analyser::visitOriginNode(ASTNode* node) {
    auto originNode = dynamic_cast<OriginNode*>(node);
    if (originNode && originNode->name.has_value()) {
        symbolTable_.insert(originNode->name.value(), originNode);
    }
}

} // namespace CHTL
