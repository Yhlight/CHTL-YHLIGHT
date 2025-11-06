#pragma once

#include "StyleContentNode.h"
#include "StylePropertyNode.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

class StyleRuleNode : public StyleContentNode {
public:
    StyleRuleNode(const std::string& selector) : selector(selector) {}

    NodeType getType() const override { return NodeType::StyleRule; }

    std::string selector;
    std::vector<std::unique_ptr<StylePropertyNode>> properties;
};

} // namespace CHTL
