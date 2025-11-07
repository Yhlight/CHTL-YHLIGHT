#pragma once

#include "BaseNode.h"
#include <string>
#include <map>

namespace CHTL {

class TemplateUsageNode;

class ElementNode : public BaseNode {
public:
    ElementNode(const std::string& tagName) : tagName(tagName) {}

    NodeType getType() const override { return NodeType::Element; }
    std::unique_ptr<BaseNode> clone() const override;
    std::unique_ptr<ElementNode> cloneWithSpecializations(const TemplateUsageNode* usageNode) const;

    std::string tagName;
    std::map<std::string, std::string> attributes;
    std::vector<std::unique_ptr<BaseNode>> children;
};

} // namespace CHTL
