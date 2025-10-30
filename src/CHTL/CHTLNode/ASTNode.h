#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include "CHTL/CHTLLexer/Token.h"

namespace CHTL {

// Forward declare node types
struct ElementNode;
struct TextNode;
struct StyleNode;
struct SelectorBlockNode;
struct ValueNode;
struct BinaryOpNode;
struct UnaryOpNode;
struct NumberLiteralNode;
struct StringLiteralNode;
struct IdentifierNode;
struct PropertyAccessNode;
struct TernaryOpNode;
struct TemplateNode;
struct TemplateUsageNode;
struct ElementSpecializationNode;
struct ImportNode;
struct NamespaceNode;
struct OriginNode;


// Enum to identify the type of an AST node
enum class NodeType {
    Program,
    Element,
    Text,
    Style,
    SelectorBlock,
    Template,
    TemplateUsage,
    ElementSpecialization,
    ElementInsertion,
	Import,
    Namespace,
    Origin,

    // Expression Nodes
    BinaryOp,
    UnaryOp,
    NumberLiteral,
    StringLiteral,
    Identifier,
    PropertyAccess,
    VarAccess,
    TernaryOp,
};

// Base class for all AST nodes
struct ASTNode {
    virtual ~ASTNode() = default;
    virtual NodeType getType() const = 0;
    virtual std::unique_ptr<ASTNode> clone() const = 0;
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
    std::unique_ptr<ASTNode> value; // Now an expression tree
};

// Represents a CSS rule block like ".class:hover { ... }"
struct SelectorBlockNode : public ASTNode {
    std::string selector;
    std::vector<StyleProperty> properties;

    NodeType getType() const override { return NodeType::SelectorBlock; }

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<SelectorBlockNode>();
        node->selector = selector;
        for (const auto& prop : properties) {
            node->properties.push_back({prop.key, prop.value->clone()});
        }
        return node;
    }

    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << selector << " {" << std::endl;
        for (const auto& prop : properties) {
            for (int j = 0; j < indent + 1; ++j) std::cout << "  ";
            std::cout << prop.key << ": " << prop.value << ";" << std::endl;
        }
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "}" << std::endl;
    }
};

// --- Expression Nodes ---

struct NumberLiteralNode : public ASTNode {
    double value;
    std::string unit;

    NodeType getType() const override { return NodeType::NumberLiteral; }
    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<NumberLiteralNode>();
        node->value = value;
        node->unit = unit;
        return node;
    }
    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "Number(" << value << unit << ")" << std::endl;
    }
};

struct StringLiteralNode : public ASTNode {
    std::string value;

    NodeType getType() const override { return NodeType::StringLiteral; }
    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<StringLiteralNode>();
        node->value = value;
        return node;
    }
    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "String(\"" << value << "\")" << std::endl;
    }
};

struct IdentifierNode : public ASTNode {
    std::string name;

    NodeType getType() const override { return NodeType::Identifier; }
    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<IdentifierNode>();
        node->name = name;
        return node;
    }
    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "Identifier(" << name << ")" << std::endl;
    }
};

struct BinaryOpNode : public ASTNode {
    std::unique_ptr<ASTNode> left;
    Token op;
    std::unique_ptr<ASTNode> right;

    NodeType getType() const override { return NodeType::BinaryOp; }
    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<BinaryOpNode>();
        node->left = left->clone();
        node->op = op;
        node->right = right->clone();
        return node;
    }
    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "BinaryOp(" << op.lexeme << "):" << std::endl;
        left->print(indent + 1);
        right->print(indent + 1);
    }
};

struct PropertyAccessNode : public ASTNode {
    std::string selector;
    std::string property;

    NodeType getType() const override { return NodeType::PropertyAccess; }
     std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<PropertyAccessNode>();
        node->selector = selector;
        node->property = property;
        return node;
    }
    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "PropertyAccess(" << selector << "." << property << ")" << std::endl;
    }
};

struct VarAccessNode : public ASTNode {
    std::string templateName;
    std::string property;

    NodeType getType() const override { return NodeType::VarAccess; }
     std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<VarAccessNode>();
        node->templateName = templateName;
        node->property = property;
        return node;
    }
    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "VarAccess(" << templateName << "." << property << ")" << std::endl;
    }
};

struct TernaryOpNode : public ASTNode {
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> then_expr;
    std::unique_ptr<ASTNode> else_expr;

    NodeType getType() const override { return NodeType::TernaryOp; }
    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<TernaryOpNode>();
        node->condition = condition->clone();
        node->then_expr = then_expr->clone();
        node->else_expr = else_expr->clone();
        return node;
    }
    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "TernaryOp:" << std::endl;
        condition->print(indent + 1);
        then_expr->print(indent + 1);
        else_expr->print(indent + 1);
    }
};

enum class TemplateType { Style, Element, Var };

struct ElementSpecializationNode : public ASTNode {
    std::string tagName;
    int index = -1; // -1 indicates no index
    std::unique_ptr<StyleNode> style;

    NodeType getType() const override { return NodeType::ElementSpecialization; }

    std::unique_ptr<ASTNode> clone() const override;
    void print(int indent = 0) const override;
};

struct ElementDeletionSpec {
    std::string tagName;
    int index = -1;
};

enum class InsertionPosition { After, Before, Replace, AtTop, AtBottom };

struct ElementInsertionNode : public ASTNode {
    InsertionPosition position;
    std::string targetTagName;
    int targetIndex = -1;
    std::vector<std::unique_ptr<ASTNode>> new_elements;

    NodeType getType() const override { return NodeType::ElementInsertion; }

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<ElementInsertionNode>();
        node->position = position;
        node->targetTagName = targetTagName;
        node->targetIndex = targetIndex;
        for (const auto& el : new_elements) {
            node->new_elements.push_back(el->clone());
        }
        return node;
    }

    void print(int indent = 0) const override {
        // TODO
    }
};

struct TemplateUsageNode : public ASTNode {
    TemplateType templateType;
    std::string name;
    std::string fromNamespace;
    std::vector<StyleProperty> provided_values;
    std::vector<std::string> deleted_properties;
    std::vector<std::unique_ptr<ElementSpecializationNode>> specializations;
    std::vector<ElementDeletionSpec> deleted_elements;
    std::vector<std::unique_ptr<ElementInsertionNode>> insertions;

    NodeType getType() const override { return NodeType::TemplateUsage; }

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<TemplateUsageNode>();
        node->templateType = templateType;
        node->name = name;
        for (const auto& prop : provided_values) {
            node->provided_values.push_back({prop.key, prop.value->clone()});
        }
        node->deleted_properties = deleted_properties;
        for (const auto& spec : specializations) {
            node->specializations.push_back(std::unique_ptr<ElementSpecializationNode>(static_cast<ElementSpecializationNode*>(spec->clone().release())));
        }
        node->deleted_elements = deleted_elements;
        for (const auto& insertion : insertions) {
            node->insertions.push_back(std::unique_ptr<ElementInsertionNode>(static_cast<ElementInsertionNode*>(insertion->clone().release())));
        }
        return node;
    }

    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "TemplateUsage(" << name << ")" << std::endl;
    }
};

struct StyleNode : public ASTNode {
    std::vector<StyleProperty> properties;
    std::vector<std::unique_ptr<SelectorBlockNode>> selector_blocks;
    std::vector<std::unique_ptr<TemplateUsageNode>> template_usages;

    NodeType getType() const override { return NodeType::Style; }

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<StyleNode>();
        for (const auto& prop : properties) {
            node->properties.push_back({prop.key, prop.value->clone()});
        }
        for (const auto& block : selector_blocks) {
            node->selector_blocks.push_back(std::unique_ptr<SelectorBlockNode>(static_cast<SelectorBlockNode*>(block->clone().release())));
        }
        for (const auto& usage : template_usages) {
            node->template_usages.push_back(std::unique_ptr<TemplateUsageNode>(static_cast<TemplateUsageNode*>(usage->clone().release())));
        }
        return node;
    }

    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "Style:" << std::endl;
        for (const auto& prop : properties) {
            for (int j = 0; j < indent + 1; ++j) std::cout << "  ";
            std::cout << prop.key << ": " << prop.value << ";" << std::endl;
        }
        for (const auto& block : selector_blocks) {
            block->print(indent + 1);
        }
    }
};

struct ElementNode : public ASTNode {
    std::string tagName;
    std::vector<Attribute> attributes;
    std::vector<std::string> auto_classes;
    std::vector<std::string> auto_ids;
    std::unique_ptr<StyleNode> style;
    std::vector<std::unique_ptr<ASTNode>> children;

    NodeType getType() const override { return NodeType::Element; }

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<ElementNode>();
        node->tagName = tagName;
        node->attributes = attributes;
        node->auto_classes = auto_classes;
        node->auto_ids = auto_ids;
        if (style) {
            node->style = std::unique_ptr<StyleNode>(static_cast<StyleNode*>(style->clone().release()));
        }
        for (const auto& child : children) {
            node->children.push_back(child->clone());
        }
        return node;
    }

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

struct TextNode : public ASTNode {
    std::string content;

    NodeType getType() const override { return NodeType::Text; }

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<TextNode>();
        node->content = content;
        return node;
    }

    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "Text(\"" << content << "\")" << std::endl;
    }
};

struct TemplateNode : public ASTNode {
    TemplateType templateType;
    std::string name;
    bool isCustom = false;
    std::vector<std::unique_ptr<ASTNode>> children;
    std::vector<StyleProperty> properties;
    std::vector<std::unique_ptr<TemplateUsageNode>> inheritances;
    std::vector<std::string> deleted_properties;

    NodeType getType() const override { return NodeType::Template; }

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<TemplateNode>();
        node->templateType = templateType;
        node->name = name;
        node->isCustom = isCustom;
        for (const auto& child : children) {
            node->children.push_back(child->clone());
        }
        for (const auto& prop : properties) {
            node->properties.push_back({prop.key, prop.value ? prop.value->clone() : nullptr});
        }
        for (const auto& inheritance : inheritances) {
            node->inheritances.push_back(std::unique_ptr<TemplateUsageNode>(static_cast<TemplateUsageNode*>(inheritance->clone().release())));
        }
        node->deleted_properties = deleted_properties;
        return node;
    }

    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << (isCustom ? "Custom(" : "Template(") << name << "):" << std::endl;
        for (const auto& child : children) {
            child->print(indent + 1);
        }
    }
};

struct ProgramNode : public ASTNode {
    std::vector<std::unique_ptr<ASTNode>> children;

    NodeType getType() const override { return NodeType::Program; }

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<ProgramNode>();
        for (const auto& child : children) {
            node->children.push_back(child->clone());
        }
        return node;
    }

    void print(int indent = 0) const override {
        std::cout << "Program:" << std::endl;
        for (const auto& child : children) {
            child->print(indent + 1);
        }
    }
};

enum class ImportType {
    Html,
    Style,
    JavaScript,
    Chtl,
    CJmod,
};

enum class ImportTemplateType {
    Element,
    Style,
    Var,
    Config,
};


struct ImportNode : public ASTNode {
    ImportType importType;
    ImportTemplateType templateType;
    bool isCustom = false;
    bool isTemplate = false;
    bool isOrigin = false;
    std::string specificName;
    std::string filePath;
    std::string alias;

    NodeType getType() const override { return NodeType::Import; }

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<ImportNode>();
        node->importType = importType;
        node->templateType = templateType;
        node->isCustom = isCustom;
        node->isTemplate = isTemplate;
        node->isOrigin = isOrigin;
        node->filePath = filePath;
        node->alias = alias;
        return node;
    }

    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "Import(file=\"" << filePath << "\", as=\"" << alias << "\")" << std::endl;
    }
};

struct NamespaceNode : public ASTNode {
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> children;

    NodeType getType() const override { return NodeType::Namespace; }

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<NamespaceNode>();
        node->name = name;
        for (const auto& child : children) {
            node->children.push_back(child->clone());
        }
        return node;
    }

    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "Namespace(" << name << ")" << std::endl;
        for (const auto& child : children) {
            child->print(indent + 1);
        }
    }
};

inline std::unique_ptr<ASTNode> ElementSpecializationNode::clone() const {
    auto node = std::make_unique<ElementSpecializationNode>();
    node->tagName = tagName;
    node->index = index;
    if (style) {
        node->style = std::unique_ptr<StyleNode>(static_cast<StyleNode*>(style->clone().release()));
    }
    return node;
}

inline void ElementSpecializationNode::print(int indent) const {
    for (int i = 0; i < indent; ++i) std::cout << "  ";
    std::cout << "ElementSpecialization(" << tagName << "[" << index << "])" << std::endl;
    if (style) {
        style->print(indent + 1);
    }
}

struct OriginNode : public ASTNode {
    std::string type;
    std::string name; // The name/alias of the block
    std::string content; // Empty if it's a usage, filled if it's a definition

    NodeType getType() const override { return NodeType::Origin; }

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<OriginNode>();
        node->type = type;
        node->name = name;
        node->content = content;
        return node;
    }

    void print(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        if (!name.empty() && content.empty()) {
            std::cout << "OriginUsage(" << type << ", name=\"" << name << "\")" << std::endl;
        } else {
            std::cout << "OriginDef(" << type << ", name=\"" << name << "\"): \"" << content << "\"" << std::endl;
        }
    }
};

} // namespace CHTL
