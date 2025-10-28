#pragma once

#include <string>
#include <vector>
#include <memory>

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class TextNode : public ASTNode {
public:
    TextNode(const std::string& text) : text(text) {}
    std::string text;
};
