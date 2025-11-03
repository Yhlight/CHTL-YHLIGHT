#pragma once

#include "ExprNode.h"
#include "SelectorExprNode.h"
#include "KeyframeNode.h"
#include <vector>
#include <memory>
#include <optional>

namespace CHTLJS {

class AnimateNode : public ExprNode {
public:
    std::unique_ptr<SelectorExprNode> target;
    int duration;
    std::string easing;
    std::vector<std::unique_ptr<KeyframeNode>> when;
    std::optional<std::vector<std::unique_ptr<CssPropertyNode>>> begin;
    std::optional<std::vector<std::unique_ptr<CssPropertyNode>>> end;
    std::optional<int> loop;
    std::optional<std::string> direction;
    std::optional<int> delay;
    std::optional<std::string> callback;


    AnimateNode(std::unique_ptr<SelectorExprNode> target, int duration, const std::string& easing)
        : target(std::move(target)), duration(duration), easing(easing) {}

    ASTNodeType getType() const override { return ASTNodeType::Animate; }
};

} // namespace CHTLJS
