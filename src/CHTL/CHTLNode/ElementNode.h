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

private:
    std::string tagName_;
    std::map<std::string, std::string> attributes_;
};
