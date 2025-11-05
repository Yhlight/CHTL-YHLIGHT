#pragma once

#include "BaseNode.h"
#include <vector>
#include <memory>

class ProgramNode : public BaseNode {
public:
    NodeType getType() const override { return NodeType::Program; }

    std::vector<std::unique_ptr<BaseNode>> children;
};
