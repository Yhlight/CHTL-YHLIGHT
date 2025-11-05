#pragma once

#include "BaseNode.h"
#include "StyleContentNode.h"
#include <vector>
#include <memory>

class StyleNode : public BaseNode {
public:
    NodeType getType() const override { return NodeType::Style; }

    std::vector<std::unique_ptr<StyleContentNode>> children;
};
