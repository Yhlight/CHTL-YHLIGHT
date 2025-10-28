#include "Analyser.h"
#include <stdexcept>
#include <sstream>

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
        default:
            // Other nodes don't need special handling in the analysis pass for now.
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

    // 1. Tag selector
    selectors.push_back(node->tagName);

    // 2. ID selector
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

    // 3. Class selectors
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

    // Recurse into children
    for (auto& child : node->children) {
        visit(child.get());
    }
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
        default:
            break;
    }
}

void Analyser::resolve(ProgramNode* node) {
    for (auto& child : node->children) {
        resolve(child.get());
    }
}

void Analyser::resolve(ElementNode* node) {
    if (node->style) {
        resolve(node->style.get());
    }
    for (auto& child : node->children) {
        resolve(child.get());
    }
}

void Analyser::resolve(StyleNode* node) {
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
        default:
            break;
    }
}

void Analyser::resolve(StyleProperty& prop) {
    resolve(prop.value);
}

} // namespace CHTL
