#pragma once

#include "BaseNode.h"
#include "BaseNode.h"
#include "TemplateUsageNode.h"
#include "DeleteNode.h"
#include <memory>
#include <string>
#include <vector>

namespace CHTL {

class TemplateNode : public BaseNode {
public:
    NodeType getType() const override { return NodeType::Template; }
    std::unique_ptr<BaseNode> clone() const override;
    std::string type;
    std::string name;
    bool isCustom = false;
    std::vector<std::unique_ptr<BaseNode>> body;
    std::vector<std::unique_ptr<TemplateUsageNode>> inheritances;
};

} // namespace CHTL
