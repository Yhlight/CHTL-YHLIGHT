#pragma once

#include "BaseNode.h"
#include <vector>
#include <memory>

namespace CHTL {

class ElementDeleteNode : public BaseNode {
public:
    NodeType getType() const override { return NodeType::ElementDelete; }

    std::vector<std::unique_ptr<BaseNode>> targets;
};

} // namespace CHTL
