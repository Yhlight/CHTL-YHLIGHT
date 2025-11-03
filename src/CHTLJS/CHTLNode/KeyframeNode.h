#pragma once

#include "ASTNode.h"
#include "CssPropertyNode.h"
#include <vector>
#include <memory>

namespace CHTLJS {

class KeyframeNode : public ASTNode {
public:
    double at;
    std::vector<std::unique_ptr<CssPropertyNode>> properties;

    KeyframeNode(double at, std::vector<std::unique_ptr<CssPropertyNode>>&& properties)
        : at(at), properties(std::move(properties)) {}

    ASTNodeType getType() const override { return ASTNodeType::Keyframe; }

    std::unique_ptr<ASTNode> clone() const override {
        std::vector<std::unique_ptr<CssPropertyNode>> newProperties;
        for (const auto& prop : properties) {
            newProperties.push_back(std::unique_ptr<CssPropertyNode>(static_cast<CssPropertyNode*>(prop->clone().release())));
        }
        return std::make_unique<KeyframeNode>(at, std::move(newProperties));
    }
};

} // namespace CHTLJS
