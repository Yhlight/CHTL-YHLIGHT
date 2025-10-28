#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <iostream>

namespace CHTL {

// Forward declare node types
struct ElementNode;
struct TextNode;

// Enum to identify the type of an AST node
enum class NodeType {
    Program,
    Element,
    Text,
    // Add other node types here as we implement them
};

// Base class for all AST nodes
struct ASTNode {
    virtual ~ASTNode() = default;
    virtual NodeType getType() const = 0;
    // A virtual method for debugging is very useful
    virtual void print(int indent = 0) const = 0;
};

// Represents a key-value attribute
struct Attribute {
    std::string key;
    std::string value;
};

// Represents an element node, like <div> or <span>
struct ElementNode : public ASTNode {
    std::string tagName;
    std::vector<Attribute> attributes;
    std::vector<std::unique_ptr<ASTNode>> children;

    NodeType getType() const override { return NodeType::Element; }

    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "<" << tagName;
        for (const auto& attr : attributes) {
            std::cout << " " << attr.key << "=\"" << attr.value << "\"";
        }
        std::cout << ">" << std::endl;

        for (const auto& child : children) {
            child->print(indent + 1);
        }

        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "</" << tagName << ">" << std::endl;
    }
};

// Represents a text node
struct TextNode : public ASTNode {
    std::string content;

    NodeType getType() const override { return NodeType::Text; }

    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "Text(\"" << content << "\")" << std::endl;
    }
};

// Represents the root of the AST
struct ProgramNode : public ASTNode {
    std::vector<std::unique_ptr<ASTNode>> children;

    NodeType getType() const override { return NodeType::Program; }

    void print(int indent = 0) const override {
        std::cout << "Program:" << std::endl;
        for (const auto& child : children) {
            child->print(indent + 1);
        }
    }
};

} // namespace CHTL
