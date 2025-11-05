#pragma once

#include <memory>
#include <vector>

enum class NodeType {
    Element,
    Text,
    Style,
    Script,
    Template,
    ElementDirective,
    StyleDirective,
    Program,
    StyleContent,
    Custom,
    // Add other node types here
};

class BaseNode {
public:
    virtual ~BaseNode() = default;
    virtual NodeType getType() const = 0;
};
