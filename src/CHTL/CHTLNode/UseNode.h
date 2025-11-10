#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>

namespace CHTL {

class UseNode : public BaseNode {
public:
    std::string type; // "html5" or "@Config"
    std::string configName;

    NodeType getType() const override {
        return NodeType::Use;
    }
};

} // namespace CHTL
