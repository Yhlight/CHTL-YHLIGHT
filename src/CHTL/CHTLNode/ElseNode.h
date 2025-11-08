#pragma once

#include "BaseNode.h"
#include "ValueNode.h"
#include <memory>
#include <vector>

namespace CHTL {

class ElseNode : public BaseNode {
public:
    std::unique_ptr<ValueNode> condition; // nullptr for else blocks
    std::vector<std::unique_ptr<BaseNode>> body;

    NodeType getType() const override { return NodeType::Else; }
};

}
