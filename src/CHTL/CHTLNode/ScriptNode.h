#pragma once

#include "BaseNode.h"
#include <string>

class ScriptNode : public BaseNode {
public:
    ScriptNode(const std::string& content) : content(content) {}

    NodeType getType() const override { return NodeType::Script; }

    std::unique_ptr<BaseNode> clone() const override {
        return std::make_unique<ScriptNode>(content);
    }

    std::string content;
};
