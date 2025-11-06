#pragma once

#include "BaseNode.h"
#include "StyleContentNode.h"

namespace CHTL {

class StyleNode : public BaseNode {
public:
    NodeType getType() const override { return NodeType::Style; }

    std::vector<std::unique_ptr<StyleContentNode>> children;
};

} // namespace CHTL
