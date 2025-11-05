#pragma once

#include "BaseNode.h"
#include <string>
#include <map>
#include <memory>

class ConfigurationNode : public BaseNode {
public:
    NodeType getType() const override { return NodeType::Configuration; }

    std::unique_ptr<BaseNode> clone() const override {
        auto node = std::make_unique<ConfigurationNode>();
        node->options = options;
        return node;
    }

    std::map<std::string, std::string> options;
};
