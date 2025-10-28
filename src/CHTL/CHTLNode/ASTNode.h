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
struct StyleNode;

// Enum to identify the type of an AST node
enum class NodeType {
    Program,
    Element,
    Text,
    Style,
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

// Represents a single CSS property
struct StyleProperty {
    std::string key;
    std::string value;
};

// Represents a style block
struct StyleNode : public ASTNode {
    std::vector<StyleProperty> properties;

    NodeType getType() const override { return NodeType::Style; }

    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "Style:" << std::endl;
        for (const auto& prop : properties) {
            for (int j = 0; j < indent + 1; ++j) std::cout << "  ";
            std::cout << prop.key << ": " << prop.value << ";" << std::endl;
        }
    }
};


// Represents an element node, like <div> or <span>
struct ElementNode : public ASTNode {
    std::string tagName;
    std::vector<Attribute> attributes;
    std::vector<std::string> auto_classes;
    std::vector<std::string> auto_ids;
    std::unique_ptr<StyleNode> style;
    std::vector<std::unique_ptr<ASTNode>> children;

    NodeType getType() const override { return NodeType::Element; }

    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "<" << tagName;
        for (const auto& attr : attributes) {
            std::cout << " " << attr.key << "=\"" << attr.value << "\"";
        }
        std::cout << ">" << std::endl;

        if (!auto_classes.empty() || !auto_ids.empty()) {
            for (int i = 0; i < indent + 1; ++i) std::cout << "  ";
            std::cout << "Auto selectors: ";
            for (const auto& c : auto_classes) std::cout << "." << c << " ";
            for (const auto& i : auto_ids) std::cout << "#" << i << " ";
            std::cout << std::endl;
        }

        if (style) {
            style->print(indent + 1);
        }

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
