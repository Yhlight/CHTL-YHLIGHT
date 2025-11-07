#pragma once

#include "BaseNode.h"
#include <string>

namespace CHTL {

class ScriptNode : public BaseNode {
public:
    ScriptNode(const std::string& scriptContent) : content(scriptContent) {}

    NodeType getType() const override { return NodeType::Script; }
    std::unique_ptr<BaseNode> clone() const override;

    std::string content;
};

} // namespace CHTL
