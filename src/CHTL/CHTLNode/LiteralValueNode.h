#pragma once

#include "ValueNode.h"
#include <string>

namespace CHTL {

class LiteralValueNode : public ValueNode {
public:
    LiteralValueNode(const std::string& value) : value(value) {}

    NodeType getType() const override { return NodeType::LiteralValue; }

    std::unique_ptr<BaseNode> clone() const override;

    std::string value;
};

} // namespace CHTL
