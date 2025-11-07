#pragma once

#include "BaseNode.h"
#include "BaseNode.h"
#include <memory>
#include <string>
#include <vector>

namespace CHTL {

class TemplateNode : public BaseNode {
public:
    NodeType getType() const override { return NodeType::Template; }
    std::string type;
    std::string name;
    std::vector<std::unique_ptr<BaseNode>> body;
};

} // namespace CHTL
