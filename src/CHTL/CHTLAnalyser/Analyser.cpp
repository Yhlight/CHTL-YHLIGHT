#include "Analyser.h"
#include "../CHTLNode/ProgramNode.h"
#include "../CHTLNode/TemplateNode.h"
#include "../CHTLNode/StyleNode.h"
#include "../CHTLNode/TemplateUsageNode.h"
#include "../CHTLNode/ElementNode.h"
#include "../CHTLNode/StylePropertyNode.h"
#include <algorithm>
#include <iostream>

namespace CHTL {

Analyser::Analyser(SymbolTable& symbolTable)
    : symbolTable_(symbolTable) {}

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
        default:
            for (const auto& child : node->children) {
                visit(child.get());
            }
            break;
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
                TemplateNode* templateNode = symbolTable_.lookup(usage->name);
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

void Analyser::visitStyleNode(ASTNode* node) {
    auto styleNode = dynamic_cast<StyleNode*>(node);
    if (!styleNode) return;
    std::vector<std::unique_ptr<ASTNode>> newChildren;

    for (auto& child : styleNode->children) {
        if (child->getType() == ASTNodeType::TemplateUsage) {
            auto usage = dynamic_cast<TemplateUsageNode*>(child.get());
            if (usage) {
                TemplateNode* templateNode = symbolTable_.lookup(usage->name);
                if (templateNode) {
                    for (const auto& prop : templateNode->children) {
                        auto styleProp = dynamic_cast<StylePropertyNode*>(prop.get());
                        if (styleProp) {
                            newChildren.push_back(std::make_unique<StylePropertyNode>(styleProp->name, styleProp->value));
                        }
                    }
                }
            }
        } else {
            newChildren.push_back(std::move(child));
        }
    }
    styleNode->children = std::move(newChildren);
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

} // namespace CHTL
