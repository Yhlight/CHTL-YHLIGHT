#include "Analyser.h"
#include "CHTLNode/TemplateNode.h"
#include "CHTLNode/TemplateUsageNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/StylePropertyNode.h"
#include "CHTLNode/LiteralValueNode.h"
#include <vector>
#include <algorithm>

Analyser::Analyser(SymbolTable& symbolTable) : symbolTable_(symbolTable) {}

void Analyser::analyse(ProgramNode& program) {
    collectTemplates(&program);
    resolveUsages(&program);
}

void Analyser::collectTemplates(ASTNode* node) {
    if (!node) return;

    std::vector<std::unique_ptr<ASTNode>>& children = node->getChildrenForModify();

    auto it = children.begin();
    while (it != children.end()) {
        ASTNode* child = it->get();
        if (child->getType() == ASTNodeType::Template) {
            TemplateNode* templateNode = static_cast<TemplateNode*>(child);
            symbolTable_.insert(templateNode->getName(), templateNode);
            ownedTemplates_.push_back(std::move(*it));
            it = children.erase(it);
        } else {
            collectTemplates(child);
            ++it;
        }
    }
}

void Analyser::resolveUsages(ASTNode* node) {
    if (!node) return;

    if (node->getType() == ASTNodeType::Style) {
        auto* styleNode = static_cast<StyleNode*>(node);
        auto& children = styleNode->getChildrenForModify();
        std::vector<std::unique_ptr<ASTNode>> newChildren;

        for (auto& child : children) {
            if (child->getType() == ASTNodeType::TemplateUsage) {
                auto* usageNode = static_cast<TemplateUsageNode*>(child.get());
                TemplateNode* templateNode = symbolTable_.lookup(usageNode->getName());
                if (templateNode) {
                    for (const auto& prop : templateNode->getChildren()) {
                        newChildren.push_back(prop->clone());
                    }
                }
            } else {
                newChildren.push_back(std::move(child));
            }
        }
        children = std::move(newChildren);
    }

    for (auto& child : node->getChildrenForModify()) {
        resolveUsages(child.get());
    }
}
