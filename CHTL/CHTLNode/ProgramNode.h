
#pragma once

#include "BaseNode.h"
#include <vector>
#include <memory>

namespace CHTL {

class ProgramNode : public BaseNode {
public:
    ProgramNode() = default;

    NodeType getType() const override { return NodeType::Program; }

    const std::vector<std::shared_ptr<BaseNode>>& getChildren() const { return m_children; }
    void addChild(std::shared_ptr<BaseNode> child) {
        if (child) {
            m_children.push_back(child);
        }
    }

private:
    std::vector<std::shared_ptr<BaseNode>> m_children;
};

} // namespace CHTL
