#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>
#include <memory>

class OriginNode : public BaseNode {
public:
    std::string type;
    std::string name;
    std::string content;

    OriginNode(std::string type, std::string name, std::string content)
        : type(std::move(type)), name(std::move(name)), content(std::move(content)) {}

    NodeType getType() const override { return NodeType::Origin; }

    std::unique_ptr<BaseNode> clone() const override {
        return std::make_unique<OriginNode>(type, name, content);
    }
};
