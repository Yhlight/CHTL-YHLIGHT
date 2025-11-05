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
    Origin,
    OriginDirective,
    Import,
    Namespace,
    Configuration,
    // Add other node types here
};

class BaseNode {
public:
    virtual ~BaseNode() = default;
    virtual NodeType getType() const = 0;
    virtual std::unique_ptr<BaseNode> clone() const = 0;
};
