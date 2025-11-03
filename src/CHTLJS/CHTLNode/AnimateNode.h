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

    std::unique_ptr<ExprNode> clone_expr() const override {
        auto newTarget = std::unique_ptr<SelectorExprNode>(static_cast<SelectorExprNode*>(target->clone().release()));
        auto newNode = std::make_unique<AnimateNode>(std::move(newTarget), duration, easing);

        if (begin.has_value()) {
            std::vector<std::unique_ptr<CssPropertyNode>> newBegin;
            for (const auto& prop : begin.value()) {
                newBegin.push_back(std::unique_ptr<CssPropertyNode>(static_cast<CssPropertyNode*>(prop->clone().release())));
            }
            newNode->begin = std::move(newBegin);
        }

        for (const auto& kf : when) {
            newNode->when.push_back(std::unique_ptr<KeyframeNode>(static_cast<KeyframeNode*>(kf->clone().release())));
        }

        if (end.has_value()) {
            std::vector<std::unique_ptr<CssPropertyNode>> newEnd;
            for (const auto& prop : end.value()) {
                newEnd.push_back(std::unique_ptr<CssPropertyNode>(static_cast<CssPropertyNode*>(prop->clone().release())));
            }
            newNode->end = std::move(newEnd);
        }

        newNode->loop = loop;
        newNode->direction = direction;
        newNode->delay = delay;
        newNode->callback = callback;

        return newNode;
    }
};

} // namespace CHTLJS
