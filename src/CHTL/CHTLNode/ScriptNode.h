#pragma once

#include "BaseNode.h"
#include <string>

class ScriptNode : public BaseNode {
public:
    ScriptNode(const std::string& content) : content(content) {}

    NodeType getType() const override { return NodeType::Script; }

    std::string content;
};
