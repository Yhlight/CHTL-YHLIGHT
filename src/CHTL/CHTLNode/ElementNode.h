#pragma once

#include "ASTNode.h"
#include <string>
#include <map>

class ElementNode : public ASTNode {
public:
    explicit ElementNode(std::string tagName) : ASTNode(ASTNodeType::Element), tagName_(std::move(tagName)) {}

    const std::string& getTagName() const { return tagName_; }

    void addAttribute(const std::string& key, const std::string& value) {
        attributes_[key] = value;
    }

    const std::map<std::string, std::string>& getAttributes() const {
        return attributes_;
    }

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<ElementNode>(tagName_);
        for (const auto& attr : attributes_) {
            node->addAttribute(attr.first, attr.second);
        }
        for (const auto& child : children_) {
            node->addChild(child->clone());
        }
        return node;
    }

private:
    std::string tagName_;
    std::map<std::string, std::string> attributes_;
};
