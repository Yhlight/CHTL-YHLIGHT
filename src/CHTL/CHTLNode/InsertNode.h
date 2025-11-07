#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

enum class InsertType {
    After,
    Before,
    Replace,
    AtTop,
    AtBottom
};

class InsertNode : public BaseNode {
public:
    InsertNode(InsertType type, const std::string& target, std::vector<std::unique_ptr<BaseNode>> body)
        : type(type), target(target), body(std::move(body)) {}

    NodeType getType() const override { return NodeType::Insert; }

    InsertType type;
    std::string target;
    std::vector<std::unique_ptr<BaseNode>> body;
};

} // namespace CHTL
