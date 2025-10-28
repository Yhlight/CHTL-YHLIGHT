#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

class TemplateElementDefinitionNode : public BaseNode {
public:
    TemplateElementDefinitionNode(const std::string& name) : m_name(name) {}

    NodeType getType() const override { return NodeType::TemplateElementDefinition; }

    const std::string& getName() const { return m_name; }
    const std::vector<std::shared_ptr<BaseNode>>& getChildren() const { return m_children; }

    void addChild(std::shared_ptr<BaseNode> child) {
        m_children.push_back(child);
    }

private:
    std::string m_name;
    std::vector<std::shared_ptr<BaseNode>> m_children;
};

} // namespace CHTL
