#pragma once

#include "BaseNode.h"
#include <vector>
#include <string>

namespace CHTL {

class ConstraintNode : public BaseNode {
public:
    std::vector<std::string> constraints;

    ConstraintNode(const std::vector<std::string>& constraints)
        : constraints(constraints) {
    }

    NodeType getType() const override { return NodeType::Constraint; }
};

} // namespace CHTL
