#include "Analyser.h"
#include <stdexcept>

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
    std::string selector;
    if (!node->auto_ids.empty()) {
        selector = "#" + node->auto_ids[0];
    } else if (!node->auto_classes.empty()) {
        selector = "." + node->auto_classes[0];
    } else {
        for (const auto& attr : node->attributes) {
            if (attr.key == "id") {
                selector = "#" + attr.value;
                break;
            }
            if (attr.key == "class") {
                selector = "." + attr.value;
                break;
            }
        }
    }

    if (!selector.empty()) {
        m_symbol_table.insert(selector, node);
        if (node->style) {
            for (const auto& prop : node->style->properties) {
                m_symbol_table.addProperty(selector, prop.key, prop.value.get());
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

    if (node->getType() == NodeType::BinaryOp) {
        auto* bin_op_node = static_cast<BinaryOpNode*>(node.get());
        resolve(bin_op_node->left);
        resolve(bin_op_node->right);
    } else if (node->getType() == NodeType::PropertyAccess) {
        auto* access_node = static_cast<PropertyAccessNode*>(node.get());
        const Symbol* symbol = m_symbol_table.find(access_node->selector);
        if (!symbol) {
            throw std::runtime_error("Unknown selector in property access: " + access_node->selector);
        }
        auto it = symbol->properties.find(access_node->property);
        if (it == symbol->properties.end()) {
            throw std::runtime_error("Unknown property in property access: " + access_node->property);
        }

        auto* referenced_value = it->second;
        if (referenced_value->getType() == NodeType::NumberLiteral) {
            auto* num_node = static_cast<const NumberLiteralNode*>(referenced_value);
            auto new_node = std::make_unique<NumberLiteralNode>();
            new_node->value = num_node->value;
            new_node->unit = num_node->unit;
            node = std::move(new_node);
        } else if (referenced_value->getType() == NodeType::StringLiteral) {
            auto* str_node = static_cast<const StringLiteralNode*>(referenced_value);
            auto new_node = std::make_unique<StringLiteralNode>();
            new_node->value = str_node->value;
            node = std::move(new_node);
        } else if (referenced_value->getType() == NodeType::Identifier) {
            auto* ident_node = static_cast<const IdentifierNode*>(referenced_value);
            auto new_node = std::make_unique<IdentifierNode>();
            new_node->name = ident_node->name;
            node = std::move(new_node);
        }
    } else if (node->getType() == NodeType::BinaryOp) {
        auto* bin_op_node = static_cast<BinaryOpNode*>(node.get());
        resolve(bin_op_node->left);
        resolve(bin_op_node->right);
    }
}

void Analyser::resolve(StyleProperty& prop) {
    resolve(prop.value);
}

} // namespace CHTL
