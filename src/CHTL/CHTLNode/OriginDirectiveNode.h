#pragma once

#include "BaseNode.h"
#include <string>

class OriginDirectiveNode : public BaseNode {
public:
    std::string type;
    std::string name;

    OriginDirectiveNode(std::string type, std::string name)
        : type(std::move(type)), name(std::move(name)) {}

    NodeType getType() const override { return NodeType::OriginDirective; }

    std::unique_ptr<BaseNode> clone() const override {
        return std::make_unique<OriginDirectiveNode>(type, name);
    }
};
