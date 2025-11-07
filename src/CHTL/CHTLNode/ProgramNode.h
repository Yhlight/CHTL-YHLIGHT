#pragma once

#include "BaseNode.h"

namespace CHTL {

class ProgramNode : public BaseNode {
public:
    NodeType getType() const override { return NodeType::Program; }
    std::unique_ptr<BaseNode> clone() const override;

    std::vector<std::unique_ptr<BaseNode>> statements;
};

} // namespace CHTL
