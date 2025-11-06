#include "Generator.h"
#include <vector>
#include <algorithm>

std::string Generator::generate(const ProgramNode& program) {
    visit(&program);
    return output_.str();
}

void Generator::visit(const ASTNode* node) {
    if (node == nullptr) return;

    switch (node->getType()) {
        case ASTNodeType::Program:
            visitProgramNode(static_cast<const ProgramNode*>(node));
            break;
        case ASTNodeType::Element:
            visitElementNode(static_cast<const ElementNode*>(node));
            break;
        case ASTNodeType::Text:
            visitTextNode(static_cast<const TextNode*>(node));
            break;
        case ASTNodeType::Style:
            visitStyleNode(static_cast<const StyleNode*>(node));
            break;
        case ASTNodeType::StyleProperty:
            visitStylePropertyNode(static_cast<const StylePropertyNode*>(node));
            break;
        case ASTNodeType::LiteralValue:
            // LiteralValueNode is handled by its parent, so we don't visit it directly.
            break;
    }
}

void Generator::visitProgramNode(const ProgramNode* node) {
    for (const auto& child : node->getChildren()) {
        visit(child.get());
    }
}

void Generator::visitElementNode(const ElementNode* node) {
    output_ << "<" << node->getTagName();

    for (const auto& attr : node->getAttributes()) {
        output_ << " " << attr.first << "=\"" << attr.second << "\"";
    }

    // Process style nodes
    std::vector<const ASTNode*> style_nodes;
    std::vector<const ASTNode*> other_nodes;
    for (const auto& child : node->getChildren()) {
        if (child->getType() == ASTNodeType::Style) {
            style_nodes.push_back(child.get());
        } else {
            other_nodes.push_back(child.get());
        }
    }

    if (!style_nodes.empty()) {
        style_buffer_.str(""); // Clear the buffer
        for (const auto& style_node : style_nodes) {
            visit(style_node);
        }
        std::string style_string = style_buffer_.str();
        if (!style_string.empty()) {
             // Remove trailing semicolon if it exists
            if (style_string.back() == ';') {
                style_string.pop_back();
            }
            output_ << " style=\"" << style_string << "\"";
        }
    }

    output_ << ">";

    for (const auto& child : other_nodes) {
        visit(child);
    }

    output_ << "</" << node->getTagName() << ">";
}

void Generator::visitTextNode(const TextNode* node) {
    output_ << node->getText();
}

void Generator::visitStyleNode(const StyleNode* node) {
    for (const auto& child : node->getChildren()) {
        visit(child.get());
    }
}

void Generator::visitStylePropertyNode(const StylePropertyNode* node) {
    const ValueNode* value_node = node->getValue();
    if (value_node && value_node->getType() == ASTNodeType::LiteralValue) {
        const auto* literal_node = static_cast<const LiteralValueNode*>(value_node);
        style_buffer_ << node->getName() << ": " << literal_node->getValue() << ";";
    }
}
