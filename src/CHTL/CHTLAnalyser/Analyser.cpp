#include "Analyser.h"
#include <stdexcept>
#include <sstream>
#include <algorithm>

namespace CHTL {

Analyser::Analyser(ProgramNode& program) : m_program(program) {}

void Analyser::analyse() {
    visit(&m_program); // First pass: collect symbols
    resolve(&m_program); // Second pass: resolve references
}

void Analyser::visit(ASTNode* node) {
    if (!node) return;

    switch (node->getType()) {
        case NodeType::Program:
            visit(static_cast<ProgramNode*>(node));
            break;
        case NodeType::Element:
            visit(static_cast<ElementNode*>(node));
            break;
        case NodeType::Template:
            visit(static_cast<TemplateNode*>(node));
            break;
        default:
            break;
    }
}

void Analyser::visit(ProgramNode* node) {
    for (auto& child : node->children) {
        visit(child.get());
    }
}

void Analyser::visit(ElementNode* node) {
    std::vector<std::string> selectors;
    selectors.push_back(node->tagName);
    std::string id_selector;
    for (const auto& attr : node->attributes) {
        if (attr.key == "id") {
            id_selector = "#" + attr.value;
            break;
        }
    }
    if (id_selector.empty() && !node->auto_ids.empty()) {
        id_selector = "#" + node->auto_ids[0];
    }
    if (!id_selector.empty()) {
        selectors.push_back(id_selector);
    }
    for (const auto& attr : node->attributes) {
        if (attr.key == "class") {
            std::istringstream iss(attr.value);
            std::string className;
            while (iss >> className) {
                selectors.push_back("." + className);
            }
        }
    }
    for (const auto& className : node->auto_classes) {
        selectors.push_back("." + className);
    }

    if (node->style) {
        for (const auto& selector : selectors) {
            if (m_symbol_table.find(selector) == nullptr) {
                m_symbol_table.insert(selector, node);
                for (const auto& prop : node->style->properties) {
                    m_symbol_table.addProperty(selector, prop.key, prop.value.get());
                }
            }
        }
    }

    for (auto& child : node->children) {
        visit(child.get());
    }
}

void Analyser::visit(TemplateNode* node) {
    if (m_templates.count(node->name)) {
        throw std::runtime_error("Template with name '" + node->name + "' already defined.");
    }
    m_templates[node->name] = node;
}

void Analyser::resolve(ASTNode* node) {
    if (!node) return;

    switch (node->getType()) {
        case NodeType::Program:
            resolve(static_cast<ProgramNode*>(node));
            break;
        case NodeType::Element:
            resolve(static_cast<ElementNode*>(node));
            break;
        case NodeType::Style:
            resolve(static_cast<StyleNode*>(node));
            break;
        case NodeType::BinaryOp:
            resolve(static_cast<BinaryOpNode*>(node)->left);
            resolve(static_cast<BinaryOpNode*>(node)->right);
            break;
        case NodeType::TemplateUsage:
            resolve(static_cast<TemplateUsageNode*>(node));
            break;
        default:
            break;
    }
}

void Analyser::resolve(ProgramNode* node) {
    std::vector<std::unique_ptr<ASTNode>> new_children;
    for (auto& child : node->children) {
        if (child->getType() == NodeType::TemplateUsage) {
            auto* usage_node = static_cast<TemplateUsageNode*>(child.get());
            if (usage_node->templateType == TemplateType::Element) {
                auto it = m_templates.find(usage_node->name);
                if (it == m_templates.end()) {
                    throw std::runtime_error("Unknown template: " + usage_node->name);
                }
                const TemplateNode* templateNode = it->second;
                for (const auto& template_child : templateNode->children) {
                    new_children.push_back(template_child->clone());
                }
            } else {
                new_children.push_back(std::move(child));
            }
        } else {
            resolve(child.get());
            new_children.push_back(std::move(child));
        }
    }
    node->children = std::move(new_children);
}

void Analyser::resolve(ElementNode* node) {
    if (node->style) {
        resolve(node->style.get());
    }

    std::vector<std::unique_ptr<ASTNode>> new_children;
    for (auto& child : node->children) {
        if (child->getType() == NodeType::TemplateUsage) {
            auto* usage_node = static_cast<TemplateUsageNode*>(child.get());
            if (usage_node->templateType == TemplateType::Element) {
                auto it = m_templates.find(usage_node->name);
                if (it == m_templates.end()) {
                    throw std::runtime_error("Unknown template: " + usage_node->name);
                }
                const TemplateNode* templateNode = it->second;
                for (const auto& template_child : templateNode->children) {
                    new_children.push_back(template_child->clone());
                }
            } else {
                 new_children.push_back(std::move(child));
            }
        } else {
            resolve(child.get());
            new_children.push_back(std::move(child));
        }
    }
    node->children = std::move(new_children);
}

void Analyser::resolve(StyleNode* node) {
    for (const auto& usage : node->template_usages) {
        if (usage->templateType == TemplateType::Style) {
            auto it = m_templates.find(usage->name);
            if (it == m_templates.end()) {
                throw std::runtime_error("Unknown style template: " + usage->name);
            }
            const TemplateNode* templateNode = it->second;
            for (const auto& prop : templateNode->properties) {
                node->properties.push_back({prop.key, prop.value->clone()});
            }
        }
    }

    for (auto& prop : node->properties) {
        resolve(prop);
    }
    for (auto& block : node->selector_blocks) {
        for (auto& prop : block->properties) {
            resolve(prop);
        }
    }
}

void Analyser::resolve(std::unique_ptr<ASTNode>& node) {
    if (!node) return;

    switch (node->getType()) {
        case NodeType::BinaryOp: {
            auto* bin_op_node = static_cast<BinaryOpNode*>(node.get());
            resolve(bin_op_node->left);
            resolve(bin_op_node->right);
            break;
        }
        case NodeType::TernaryOp: {
            auto* ternary_node = static_cast<TernaryOpNode*>(node.get());
            resolve(ternary_node->condition);
            resolve(ternary_node->then_expr);
            resolve(ternary_node->else_expr);
            break;
        }
        case NodeType::PropertyAccess: {
            auto* access_node = static_cast<PropertyAccessNode*>(node.get());
            const Symbol* symbol = m_symbol_table.find(access_node->selector);
            if (!symbol) {
                throw std::runtime_error("Unknown selector in property access: " + access_node->selector);
            }
            auto it = symbol->properties.find(access_node->property);
            if (it == symbol->properties.end()) {
                throw std::runtime_error("Unknown property in property access: " + access_node->property);
            }
            node = it->second->clone();
            break;
        }
        case NodeType::VarAccess: {
            auto* var_node = static_cast<VarAccessNode*>(node.get());
            auto it = m_templates.find(var_node->templateName);
            if (it == m_templates.end() || it->second->templateType != TemplateType::Var) {
                throw std::runtime_error("Unknown var template: " + var_node->templateName);
            }
            const TemplateNode* templateNode = it->second;
            auto prop_it = std::find_if(templateNode->properties.begin(), templateNode->properties.end(),
                                        [&](const StyleProperty& prop) { return prop.key == var_node->property; });
            if (prop_it == templateNode->properties.end()) {
                throw std::runtime_error("Unknown property '" + var_node->property + "' in var template '" + var_node->templateName + "'");
            }
            node = prop_it->value->clone();
            break;
        }
        default:
            break;
    }
}

void Analyser::resolve(TemplateUsageNode* node) {
    auto it = m_templates.find(node->name);
    if (it == m_templates.end()) {
        throw std::runtime_error("Unknown template: " + node->name);
    }
    const TemplateNode* templateNode = it->second;

    // This is a placeholder. In a real implementation, you would
    // expand the template here. For now, we'll just check for its existence.
}

void Analyser::resolve(StyleProperty& prop) {
    resolve(prop.value);
}

}
