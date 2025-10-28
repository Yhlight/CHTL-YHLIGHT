#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class TextNode : public ASTNode {
public:
    TextNode(const std::string& text) : text(text) {}
    std::string text;
};

struct StyleBlockEntry {
    std::string value;
};

class StyleNode : public ASTNode {
public:
    std::unordered_map<std::string, StyleBlockEntry> properties;
};

class ElementNode : public ASTNode {
public:
    ElementNode(const std::string& tagName) : tagName(tagName) {}
    std::string tagName;
    std::unordered_map<std::string, std::string> attributes;
    std::vector<std::unique_ptr<ASTNode>> children;
};
