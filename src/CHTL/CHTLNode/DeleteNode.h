#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>

namespace CHTL {

class DeleteNode : public BaseNode {
public:
    NodeType getType() const override { return NodeType::Delete; }

    std::vector<std::string> properties;
    std::vector<std::string> inheritances;
};

} // namespace CHTL
