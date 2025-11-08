#pragma once

#include "BaseNode.h"
#include "ValueNode.h"
#include "ElseNode.h"
#include <memory>
#include <vector>

namespace CHTL {

class IfNode : public BaseNode {
public:
    std::unique_ptr<ValueNode> condition;
    std::vector<std::unique_ptr<BaseNode>> body;
    std::vector<std::unique_ptr<ElseNode>> else_branches;

    NodeType getType() const override { return NodeType::If; }
};

}
