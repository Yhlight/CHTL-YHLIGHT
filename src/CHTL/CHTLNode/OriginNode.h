#pragma once

#include "BaseNode.h"
#include <string>

namespace CHTL {

class OriginNode : public BaseNode {
public:
    OriginNode(const std::string& type, const std::string& content, const std::string& name = "") : originType(type), content(content), name(name) {}

    NodeType getType() const override { return NodeType::Origin; }

    std::string originType;
    std::string content;
    std::string name;
};

} // namespace CHTL
