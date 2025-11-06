#include "Analyser.h"
#include "CHTLNode/TemplateNode.h"
#include "CHTLNode/TemplateUsageNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/StylePropertyNode.h"
#include "CHTLNode/LiteralValueNode.h"
#include "CHTLNode/ImportNode.h"
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include <vector>
#include <algorithm>
#include <iostream>

Analyser::Analyser(SymbolTable& symbolTable) : symbolTable_(symbolTable) {}

void Analyser::analyse(ProgramNode& program) {
    resolveImports(&program);
    collectTemplates(&program);
    resolveUsages(&program);
}

void Analyser::resolveImports(ASTNode* node) {
    if (!node) return;

    std::vector<std::unique_ptr<ASTNode>>& children = node->getChildrenForModify();

    auto it = children.begin();
    while (it != children.end()) {
        ASTNode* child = it->get();
        if (child->getType() == ASTNodeType::Import) {
            auto* importNode = static_cast<ImportNode*>(child);
            std::filesystem::path path = importNode->getPath();

            if (std::find(importStack_.begin(), importStack_.end(), path) != importStack_.end()) {
                std::cerr << "Error: Circular import detected: " << path << std::endl;
                it = children.erase(it);
                continue;
            }

            importStack_.push_back(path);

            try {
                std::string source = importer_.read(path);
                Lexer lexer(source);
                Parser parser(lexer);
                auto program = parser.parse();
                if (program) {
                    analyse(*program);
                }
            } catch (const std::runtime_error& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }

            importStack_.pop_back();
            it = children.erase(it);
        } else {
            resolveImports(child);
            ++it;
        }
    }
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
