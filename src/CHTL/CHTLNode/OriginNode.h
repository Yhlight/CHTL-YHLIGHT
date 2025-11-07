#pragma once

#include "BaseNode.h"
#include <string>

namespace CHTL {

class OriginNode : public BaseNode {
public:
    OriginNode(const std::string& type, const std::string& content) : originType(type), content(content) {}

    NodeType getType() const override { return NodeType::Origin; }
    std::unique_ptr<BaseNode> clone() const override;

    std::string originType;
    std::string content;
};

} // namespace CHTL
