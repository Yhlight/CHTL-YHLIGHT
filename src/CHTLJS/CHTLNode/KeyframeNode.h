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
};

} // namespace CHTLJS
