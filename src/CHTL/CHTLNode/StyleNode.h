#pragma once

#pragma once

#include "BaseNode.h"
#include "StyleContentNode.h"
#include <string>
#include <vector>

namespace CHTL {

class StyleNode : public BaseNode {
public:
    NodeType getType() const override { return NodeType::Style; }

    std::vector<std::unique_ptr<StyleContentNode>> children;
    std::vector<std::string> styleGroupReferences;
};

} // namespace CHTL
