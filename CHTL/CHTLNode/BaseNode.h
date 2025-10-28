#pragma once

#include <memory>
#include <vector>

namespace CHTL {

enum class NodeType {
    Program,
    Element,
    Text,
    Attribute,
    StyleBlock,
    TemplateStyleDefinition,
    TemplateStyleUsage,
    // ... other node types
};

class BaseNode {
public:
    virtual ~BaseNode() = default;
    virtual NodeType getType() const = 0;
};

} // namespace CHTL
