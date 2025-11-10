#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

class ConfigNode : public BaseNode {
public:
    std::string name;
    std::vector<std::pair<std::string, std::string>> settings;

    ConfigNode() {}

    NodeType getType() const override { return NodeType::Config; }
    std::string ToString() const override {
        return "ConfigNode";
    }
};

} // namespace CHTL
