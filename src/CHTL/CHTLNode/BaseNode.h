#pragma once

#include <memory>
#include <vector>

namespace CHTL {

enum class NodeType {
    Program,
    Element,
};

class BaseNode {
public:
    virtual ~BaseNode() = default;
    virtual NodeType getType() const = 0;
};

} // namespace CHTL
