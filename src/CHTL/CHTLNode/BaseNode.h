#pragma once

#include <memory>
#include <vector>

enum class NodeType {
    Element,
    Text,
    Style,
    Script,
    // Add other node types here
};

class BaseNode {
public:
    virtual ~BaseNode() = default;
    virtual NodeType getType() const = 0;
};
